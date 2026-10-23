/*
 * A018556  Divisors of 624.
 * https://oeis.org/A018556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 624 (sorted in increasing order)
 * Range:     u8 n <= 14, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Helper: initialise and sort the list of divisors of 624 */
static void init_divisors(void)
{
    /* 624 = 2^4 * 3^1 * 13^1 -> (4+1)*(1+1)*(1+1) = 20 divisors */
    seq_t divisors[20];
    size_t idx = 0;

    uint64_t pow2_u = 1;                /* 2^0 */
    for (idx_t a = 0; a <= 4; ++a) {
        seq_t pow2 = (seq_t)pow2_u;
        seq_t pow3 = (seq_t)1;
        seq_t pow13 = (seq_t)1;

        /* b = 0 */
        for (idx_t c = 0; c <= 1; ++c) {
            divisors[idx++] = pow2 * (pow3) * (pow13);
            pow13 = (c == 0) ? (seq_t)13 : pow13; /* set for next c */
        }

        pow3 = (seq_t)3;                /* b = 1 */
        pow13 = (seq_t)1;
        for (idx_t c = 0; c <= 1; ++c) {
            divisors[idx++] = pow2 * (pow3) * (pow13);
            pow13 = (c == 0) ? (seq_t)13 : pow13;
        }

        if (a < 4) pow2_u *= 2;          /* next power of two */
    }

    /* Simple selection sort into static storage */
    for (size_t i = 0; i < 20; ++i) {
        for (size_t j = i + 1; j < 20; ++j) {
            if (divisors[j] < divisors[i]) {
                seq_t tmp = divisors[i];
                divisors[i] = divisors[j];
                divisors[j] = tmp;
            }
        }
    }

    /* Copy sorted divisors into a static array for fast lookup */
    static seq_t cache[20];
    for (size_t i = 0; i < 20; ++i)
        cache[i] = divisors[i];

    /* Store the cache in static storage so subsequent calls reuse it */
    static seq_t *static_cache = NULL;
    if (static_cache == NULL) {
        static_cache = cache;
    }
}

/* External linkage function required by the framework */
seq_t a(idx_t n)
{
    /* Initialise divisor list once */
    static bool init_done = false;
    static seq_t cache[20];

    if (!init_done) {
        /* Populate cache */
        seq_t tmp[20];
        uint64_t pow2_u = 1;
        size_t idx = 0;
        for (idx_t a = 0; a <= 4; ++a) {
            seq_t pow2 = (seq_t)pow2_u;
            seq_t pow3 = (seq_t)1;
            seq_t pow13 = (seq_t)1;

            for (idx_t c = 0; c <= 1; ++c) {
                tmp[idx++] = pow2 * pow3 * pow13;
                pow13 = (c == 0) ? (seq_t)13 : pow13;
            }

            pow3 = (seq_t)3;
            pow13 = (seq_t)1;
            for (idx_t c = 0; c <= 1; ++c) {
                tmp[idx++] = pow2 * pow3 * pow13;
                pow13 = (c == 0) ? (seq_t)13 : pow13;
            }

            if (a < 4) pow2_u *= 2;
        }

        /* Simple selection sort */
        for (size_t i = 0; i < 20; ++i) {
            for (size_t j = i + 1; j < 20; ++j) {
                if (tmp[j] < tmp[i]) {
                    seq_t tmp_val = tmp[i];
                    tmp[i] = tmp[j];
                    tmp[j] = tmp_val;
                }
            }
        }

        for (size_t i = 0; i < 20; ++i)
            cache[i] = tmp[i];

        init_done = true;
    }

    /* Boundary check: the sequence has exactly 20 values */
    if (n == 0) return 0;          /* out of bounds, but the build never calls this */
    if (n <= 20) return cache[n - 1];
    return 0;                      /* for n > 20 the sequence is undefined */
}
