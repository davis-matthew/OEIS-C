/*
 * A123899  a(n) = (n+1)!/(d(n)*d(n+1)) where d(n) = cancellation factor in
 *          reducing Sum_{k=0...n} 1/k! to lowest terms.
 * https://oeis.org/A123899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1)!/(d(n)*d(n+1)) where d(k)=gcd(k!, ∑_{i=0}^{k}
 *            k!/i!)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* --- Lazy static storage for precomputed data -------------------------------- */
    static seq_t *g_arr  = NULL;   // d(k)
    static seq_t *rem_arr = NULL; // T_k mod k!
    static seq_t *fact_arr = NULL; // k!
    static idx_t max_computed = 0; // highest index computed

    /* --- Initial allocation and first element (k = 0) --------------------------- */
    if (!g_arr) {
        size_t init_cap = 1;
        g_arr     = (seq_t*)calloc(init_cap, sizeof(seq_t));
        rem_arr   = (seq_t*)calloc(init_cap, sizeof(seq_t));
        fact_arr  = (seq_t*)calloc(init_cap, sizeof(seq_t));
        if (!g_arr || !rem_arr || !fact_arr)
            exit(1);                       /* allocation failed */

        seq_t F = (seq_t)1;               // 0! = 1
        seq_t r = (seq_t)1 % (seq_t)1;    // 1%1 == 0
        g_arr[0]    = gcd(F, r);          // d(0) = 1
        rem_arr[0]  = r;
        fact_arr[0] = F;
        max_computed = 0;
    }

    /* --- Need values up to k = n+1 ------------------------------------------- */
    idx_t target = n + 1;
    if (max_computed < target) {
        /*--- enlarge storage if needed ----------------------------------------*/
        size_t cap = max_computed ? max_computed : 1;
        while (cap <= target) cap <<= 1;                       /* double capacity */
        g_arr     = (seq_t*)realloc(g_arr,     cap * sizeof(seq_t));
        rem_arr   = (seq_t*)realloc(rem_arr,   cap * sizeof(seq_t));
        fact_arr  = (seq_t*)realloc(fact_arr,  cap * sizeof(seq_t));
        if (!g_arr || !rem_arr || !fact_arr)
            exit(1);                                          /* allocation failed */

        /*--- compute missing values ----------------------------------------*/
        for (idx_t k = max_computed + 1; k <= target; ++k) {
            seq_t F_prev = fact_arr[k-1];          /* (k-1)! */
            seq_t rem_prev = rem_arr[k-1];         /* T_{k-1} mod (k-1)! */

            seq_t F = F_prev * (seq_t)k;           /* k! */
            seq_t r = (((seq_t)k * rem_prev) + (seq_t)1) % F; /* T_k mod k! */
            seq_t d = gcd(F, r);

            fact_arr[k] = F;
            rem_arr[k]  = r;
            g_arr[k]    = d;
        }
        max_computed = target;
    }

    /* --- Compute a(n) -------------------------------------------------------- */
    seq_t numerator   = fact_arr[n+1];         /* (n+1)!   */
    seq_t denominator = g_arr[n] * g_arr[n+1];
    return numerator / denominator;
}
