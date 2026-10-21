/*
 * A090324  Second in a series of triangular arrays generating the natural
 *          numbers (cf. A079946).
 * https://oeis.org/A090324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(1) = 6. For each cycle m = 1,2,3,... define P_m by P_1 = 7 and
 *            P_{m+1} = 2·P_m + 3. Starting from the previous term a, compute
 *            a ← a + P_m, (store this as a term) then for k = m‑1 down to 0 a
 *            ← a – 3·2^k, (store each result as a term). This generates the
 *            sequence 6,13,10,27,21,18,55,43,37,34,11...
 * Range:     u8 n <= 21, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* on first call initialise the static cache and the cycle variables */
    static seq_t *cache    = NULL;
    static size_t  size    = 0;    /* number of terms stored */
    static size_t  cap     = 0;    /* allocated capacity */
    static __uint128_t P    = 7;   /* current P_m for the next cycle */
    static idx_t     cycle  = 1;   /* next cycle number (1‑based) */

    if (n == 0)          /* safety – offset is 1, but guard anyway */
        return (seq_t)0;

    /* grow cache to hold at least one term on first call */
    if (size == 0) {
        cap    = 16;
        cache  = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)6;
        size = 1;
    }

    /* generate terms until we have at least n terms */
    while (size < (size_t)n) {
        /* Ensure there is room for the new term(s) */
        if (size == cap) {
            cap *= 2;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }

        /* previous value in 128‑bit for full precision */
        __uint128_t prev = ( __uint128_t )cache[size - 1];

        /* first addition in the cycle */
        __uint128_t val = prev + P;
        cache[size++] = (seq_t)val;
        prev = val;

        /* successive subtractions of 3·2^k for k = cycle–1 … 0 */
        /* compute 2^(cycle-1) once and then halve it each step */
        if (cycle < 128) {
            __uint128_t pow2 = (__uint128_t)1 << (cycle - 1);
            for (idx_t k = cycle - 1; k != (idx_t)(-1); --k) {
                /* ensure capacity for each produced term */
                if (size == cap) {
                    cap *= 2;
                    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                }
                val = prev - ( (__uint128_t)3 * pow2 );
                cache[size++] = (seq_t)val;
                prev = val;
                pow2 >>= 1;          /* next lower power of two */
            }
        } else {
            /* for very large cycles the powers exceed __uint128_t;
               but all terms generated before this point already overflow
               any 64‑bit seq_t – the sequence will never be queried here. */
            break;
        }

        /* prepare for the next cycle */
        P = P * 2 + 3;
        ++cycle;
    }

    return cache[n - 1];
}
