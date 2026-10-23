/*
 * A022338  Index of 5^n within sequence of numbers of form 3^i*5^j.
 * https://oeis.org/A022338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 299, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A022338: index of 5^n in the sorted list of numbers 3^i*5^j.
   Recurrence: a(1)=1;
               a(n)=a(n-1)+floor((n-1)*log_3(5))+1   (n>1)
   Here log_3(5) is approximated by the rational NUM/DEN.           */

#include <stdlib.h>
#include <stdint.h>

/* high‑precision rational approximation of log_3(5) = ln(5)/ln(3)
   NUM/DEN ≈ 1.464973520059731...                                        */
static const unsigned long long NUM = 1464973520060ULL; /* 1.464973520060 × 10^12 */
static const unsigned long long DEN = 1000000000000ULL; /* 10^12             */

/* memoisation array for a(n). prefix[0] is a dummy; prefix[1] = 1.   */
static seq_t *prefix = NULL;
static idx_t  capacity = 0;

seq_t a(idx_t n)
{
    if (n == 0)          /* defined for n≥1; return 0 for n=0 */
        return 0;

    /* initialise memoisation structure if needed */
    if (prefix == NULL) {
        capacity = 2;                            /* indices 0 and 1 */
        prefix = (seq_t *)malloc(sizeof(seq_t) * capacity);
        if (!prefix)
            return 0;                      /* allocation failure safe‑exit */
        prefix[0] = 0;
        prefix[1] = 1;
    }

    /* grow the memoisation array until it contains a(n) */
    while (capacity <= n) {
        idx_t new_cap = capacity * 2;
        if (new_cap <= n)   /* ensure at least n+1 entries */
            new_cap = n + 1;

        seq_t *tmp = (seq_t *)realloc(prefix, sizeof(seq_t) * new_cap);
        if (!tmp)
            return 0;                    /* allocation failure safe‑exit */
        prefix = tmp;

        for (idx_t i = capacity; i < new_cap; ++i) {
            /* compute floor((i-1)*NUM/DEN) using 128‑bit arithmetic */
            unsigned __int128 prod = (unsigned __int128)(i - 1) * NUM;
            seq_t floor_t = (seq_t)(prod / DEN);
            prefix[i] = prefix[i - 1] + floor_t + 1;
        }
        capacity = new_cap;
    }

    return prefix[n];
}
