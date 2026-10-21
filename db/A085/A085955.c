/*
 * A085955  Composites such that the number of 1's in their binary expansion
 *          is equal to the number of 1's in the binary expansion of the sum
 *          of their prime factors (counting repetition).
 * https://oeis.org/A085955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 5540, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the nth composite m such that
   the number of 1‑bits in the binary expansion of m equals
   the number of 1‑bits in the binary expansion of the sum of
   its prime factors (counted with multiplicity).
   For example a(42) = 216 because 216 = '11011000' and
   sopfr(216) = 15 = '1111', both having four 1’s. */

#include <stdint.h>
#include <stdlib.h>

/* Count 1-bits of a 64‑bit value. */
static inline unsigned popcount_u64(uint64_t v)
{
    /* __builtin_popcountll is available in both C11 and C++17. */
    return __builtin_popcountll(v);
}

/* Return the nth term of the sequence.  The results are cached
   so that calls with arbitrary n are efficient. */
seq_t a(idx_t n)
{
    if (n == 0)      /* the sequence is indexed from 1 */
        return 0;

    /* Static cache of found terms. */
    static seq_t *cache = NULL;   /* array of terms */
    static size_t   sz   = 0;      /* number of cached terms */
    static size_t   cap  = 0;      /* current capacity of cache */
    static uint64_t cur  = 4;      /* next integer to test, start at the first composite */

    while (sz < n) {              /* find terms until we have n of them */
        uint64_t m = cur++;       /* candidate integer */

        /* Factor m and determine if it is composite. */
        uint64_t temp = m;
        uint64_t sum  = 0;
        int      divided = 0;     /* becomes 1 if we removed at least one prime factor */

        /* Handle factor 2 separately. */
        if ((temp & 1ULL) == 0) {
            divided = 1;
            while ((temp & 1ULL) == 0) {
                sum += 2;
                temp >>= 1;       /* divide by 2 */
            }
        }

        /* Test odd divisors up to sqrt(temp). */
        for (uint64_t i = 3; i * i <= temp; i += 2) {
            if (temp % i == 0) {
                divided = 1;
                while (temp % i == 0) {
                    sum += i;
                    temp /= i;
                }
            }
        }

        /* If a remaining factor > 1 was found, it is prime. */
        if (temp > 1) {
            /* If we have not divided at all, m itself is prime. */
            if (!divided && m != temp) {
                /* Prime number – skip it. */
                continue;
            }
            sum += temp;
        }

        /* If no division occurred, m is prime – skip. */
        if (!divided)
            continue;

        /* Compare popcounts of m and its prime‑factor sum. */
        if (popcount_u64(m) == popcount_u64(sum)) {
            /* Extend cache if necessary. */
            if (sz == cap) {
                size_t newcap = cap == 0 ? 16 : cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) abort();   /* allocation failure – cannot recover */
                cache = tmp;
                cap   = newcap;
            }
            cache[sz++] = (seq_t)m;
        }
    }

    return cache[n - 1];
}
