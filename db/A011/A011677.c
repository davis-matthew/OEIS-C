/*
 * A011677  A binary m-sequence: expansion of reciprocal of x^7+x^5+x^2+x+1.
 * https://oeis.org/A011677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) ⊕ a(n-2) ⊕ a(n-5) ⊕ a(n-7) with initial values:
 *            a(0) = 0 a(1) = 0 a(2) = 0 a(3) = 0 a(4) = 0 a(5) = 0 a(6) = 1
 *            The sequence is 127‑periodic
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t seq_cache[127];
static bool seq_init = false;

static void init_seq(void)
{
    seq_cache[0] = 0;
    seq_cache[1] = 0;
    seq_cache[2] = 0;
    seq_cache[3] = 0;
    seq_cache[4] = 0;
    seq_cache[5] = 0;
    seq_cache[6] = 1;
    for (idx_t i = 7; i < 127; ++i) {
        seq_cache[(size_t)i] =
            seq_cache[(size_t)(i - 1)] ^
            seq_cache[(size_t)(i - 2)] ^
            seq_cache[(size_t)(i - 5)] ^
            seq_cache[(size_t)(i - 7)];
    }
    seq_init = true;
}

seq_t a(idx_t n)
{
    if (!seq_init) init_seq();
    return seq_cache[(size_t)(n % 127u)];
}
