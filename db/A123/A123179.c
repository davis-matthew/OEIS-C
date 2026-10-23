/*
 * A123179  a(1)=1 a(2)=121 and a(n)=a(n-1) n 2 n 3 n ... n (n-1) n 1, using
 *          concatenation.
 * https://oeis.org/A123179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A123179: sequence defined by
   a(1) = 1
   a(2) = 121
   a(n) = concat( a(n-1) , n2 n3 … n(n‑1) n1 )           */
/* where “concat” means decimal concatenation of the numbers */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/digits.h"   /* int digits(idx_t) */
#include "../utils/pow10.h"    /* seq_t pow10(idx_t) */

seq_t a(idx_t n)
{
    if (n == 0)           /* offset is 1, this case is irrelevant */
        return (seq_t)0;

    /* static storage for memoisation */
    static seq_t *cache = NULL;
    static seq_t *cumDigits = NULL;   /* cumDigits[i] = sum_{k=1}^{i} digits(k) */
    static size_t cap = 0;
    static idx_t max_cached = 0;

    /* grow arrays if needed */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 2;   /* +2 for safety when n == 0 */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cumDigits = (seq_t *)realloc(cumDigits, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* initialise base cases */
    if (max_cached == 0) {               /* first call builds base */
        cache[1] = (seq_t)1;
        cumDigits[1] = (seq_t)digits(1);
        max_cached = 1;
    }
    if (max_cached < 2 && n >= 2) {      /* prepare a(2) if needed */
        cache[2] = (seq_t)121;
        cumDigits[2] = cumDigits[1] + (seq_t)digits(2);
        max_cached = 2;
    }

    /* fill in missing values up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        /* compute the concatenated part for this i */
        seq_t appended = (seq_t)0;
        seq_t shiftPart = (seq_t)0;
        int dig_i = digits(i);

        /* append blocks n-k for k=2..i-1 */
        for (idx_t k = 2; k <= i - 1; ++k) {
            int dig_k = digits(k);
            seq_t block = (seq_t)i * pow10((idx_t)dig_k) + (seq_t)k;
            seq_t blockPower = pow10((idx_t)(dig_i + dig_k));
            appended = appended * blockPower + block;
        }
        /* final block n-1 */
        /* at this step k = 1 for last block */
        seq_t block = (seq_t)i * pow10((idx_t)1) + (seq_t)1;
        seq_t blockPower = pow10((idx_t)(dig_i + 1));
        appended = appended * blockPower + block;

        /* compute number of digits in the appended part:
           (i-1)*digits(i) + sum_{k=1}^{i-1} digits(k) */
        seq_t digitsAppended = ((seq_t)(i - 1) * dig_i) + cumDigits[i - 1];

        seq_t shift = pow10((idx_t)digitsAppended);

        cache[i] = cache[i - 1] * shift + appended;

        /* update cumulative digits sum */
        cumDigits[i] = cumDigits[i - 1] + (seq_t)dig_i;
    }

    max_cached = n;
    return cache[n];
}
