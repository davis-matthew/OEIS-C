/*
 * A033870  Divisors = 1 (mod 4) of Descartes's 198585576189.
 * https://oeis.org/A033870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, nonn, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 231, u64 n <= 246,
 *            u128 n <= 246, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence: a(n) is the n-th divisor of 198 585 576 189 that is congruent to 1 (mod 4), sorted increasingly. */

#include <stddef.h>
#include <stdlib.h>

static seq_t cache[250];
static idx_t cache_count = 0;
static int cache_ready = 0;

static int cmp_seq(const void *a, const void *b)
{
    const seq_t x = *(const seq_t *)a;
    const seq_t y = *(const seq_t *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* sequence starts at n = 1 */
    if (!cache_ready)
    {
        /* The prime factorisation of 198 585 576 189.  The exponent for 61 is 1, not 2. */
        const seq_t primes[6] = {3, 7, 11, 13, 19, 61};
        const int exps[6]   = {2, 2, 2, 2, 2, 1};
        seq_t divs[486];
        unsigned idx = 0;

        for (int e0 = 0; e0 <= exps[0]; ++e0) {
            seq_t p0 = 1;
            for (int i = 0; i < e0; ++i) p0 *= primes[0];
            for (int e1 = 0; e1 <= exps[1]; ++e1) {
                seq_t p1 = p0;
                for (int i = 0; i < e1; ++i) p1 *= primes[1];
                for (int e2 = 0; e2 <= exps[2]; ++e2) {
                    seq_t p2 = p1;
                    for (int i = 0; i < e2; ++i) p2 *= primes[2];
                    for (int e3 = 0; e3 <= exps[3]; ++e3) {
                        seq_t p3 = p2;
                        for (int i = 0; i < e3; ++i) p3 *= primes[3];
                        for (int e4 = 0; e4 <= exps[4]; ++e4) {
                            seq_t p4 = p3;
                            for (int i = 0; i < e4; ++i) p4 *= primes[4];
                            for (int e5 = 0; e5 <= exps[5]; ++e5) {
                                seq_t p5 = p4;
                                for (int i = 0; i < e5; ++i) p5 *= primes[5];
                                divs[idx++] = p5;
                            }
                        }
                    }
                }
            }
        }

        /* Keep only those congruent to 1 (mod 4) and sort them */
        for (unsigned i = 0; i < idx; ++i) {
            if ((divs[i] % (seq_t)4) == (seq_t)1) {
                cache[cache_count++] = divs[i];
            }
        }

        qsort(cache, cache_count, sizeof(seq_t), cmp_seq);
        cache_ready = 1;
    }

    if (n <= (idx_t)cache_count) {
        return cache[n - 1];
    }
    return (seq_t)0;   /* undefined for indices beyond the finite sequence */
}
