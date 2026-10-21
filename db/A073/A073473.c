/*
 * A073473  Primes (including 1) forming 3 X 3 magic square with prime entries
 *          and minimal constant 111 = A073502(3).
 * https://oeis.org/A073473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 none, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   is_prime_small, sort_seq_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find the unique set of 9 primes (including 1) that form a 3×3 magic square with constant 111 */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_small.h"
#include "../utils/sort_seq_seq.h"

static inline bool is_prime_or_one(seq_t x)
{
    return x == 1 ? true : is_prime_small((idx_t)x);
}

seq_t a(idx_t n)
{
    /* static cache of the result */
    static bool initialized = false;
    static seq_t primes_arr[9];
    static idx_t count = 0;

    if (!initialized) {
        /* candidate primes including 1 up to 73 */
        static const seq_t cand[] = {
            1, 2, 3, 5, 7, 11, 13, 17, 19, 23,
            29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73
        };
        static const size_t candcnt = sizeof(cand) / sizeof(cand[0]);

        bool found = false;
        for (size_t ai = 0; ai < candcnt && !found; ++ai) {
            seq_t a_val = cand[ai];
            for (size_t di = 0; di < candcnt && !found; ++di) {
                if (di == ai) continue;
                seq_t d_val = cand[di];

                /* c = a + d - 37, must be prime and distinct */
                seq_t c_val = a_val + d_val - 37;
                if (c_val <= 0 || c_val > 73 || !is_prime_or_one(c_val))
                    continue;
                if (c_val == a_val || c_val == d_val)
                    continue;

                seq_t f_val = 74 - d_val;
                if (f_val <= 0 || !is_prime_or_one(f_val))
                    continue;
                if (f_val == a_val || f_val == d_val || f_val == c_val)
                    continue;

                seq_t g_val = 74 - c_val;
                if (g_val <= 0 || !is_prime_or_one(g_val))
                    continue;
                if (g_val == a_val || g_val == d_val || g_val == c_val || g_val == f_val)
                    continue;

                seq_t b_val = 111 - a_val - c_val;
                if (b_val <= 0 || !is_prime_or_one(b_val))
                    continue;
                if (b_val == a_val || b_val == d_val || b_val == c_val || b_val == f_val ||
                    b_val == g_val || b_val == 37)
                    continue;

                seq_t h_val = 74 - b_val;
                if (h_val <= 0 || !is_prime_or_one(h_val))
                    continue;
                if (h_val == a_val || h_val == d_val || h_val == c_val || h_val == f_val ||
                    h_val == g_val || h_val == b_val || h_val == 37)
                    continue;

                seq_t i_val = 74 - a_val;
                if (i_val <= 0 || !is_prime_or_one(i_val))
                    continue;
                if (i_val == a_val || i_val == d_val || i_val == c_val || i_val == f_val ||
                    i_val == g_val || i_val == b_val || i_val == h_val || i_val == 37)
                    continue;

                /* all distinct, store */
                seq_t tmp[9] = { a_val, b_val, c_val, d_val, 37, f_val, g_val, h_val, i_val };
                sort_seq_seq(tmp, 9);
                for (size_t k = 0; k < 9; ++k)
                    primes_arr[k] = tmp[k];
                count = 9;
                found = true;
            }
        }
        initialized = true;
    }

    if (n == 0 || !initialized || n > count)
        return 0;
    return primes_arr[n - 1];
}
