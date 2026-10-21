/*
 * A072409  4th-order digital invariants: the sum of the 4th power of the
 *          digits of n equals some number k and the sum of the 4th power of
 *          the digits of k equals n.
 * https://oeis.org/A072409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   a(n) = nth smallest integer k>0 such that the sum of the 4th
 *            powers of * its decimal digits equals some integer k', and the
 *            sum of the 4th powers of * the digits of k' equals k
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compile with any C11 or C++17 compiler.  seq_t and idx_t are predefined
 * by the build system.  The code may be compiled with seq_t of any size
 * (8, 16, 32, 64 or 128 bits or an arbitrary‑precision type) and with
 * idx_t a 64‑bit unsigned integer. */

#include <stddef.h>
#include <stdbool.h>

#define MAXN 100000   /* covers all n that can satisfy f(f(n)) == n */

static const seq_t POW4[10] = {0, 1, 16, 81, 256, 625, 1296, 2401,
                               4096, 6561};

static seq_t f[MAXN + 1];
static seq_t seq_results[MAXN + 1];
static size_t seq_count = 0;
static bool initialized = false;

static void compute_f(void)
{
    for (idx_t i = 0; i <= MAXN; ++i) {
        seq_t x = (seq_t)i;
        seq_t sum = 0;
        while (x > 0) {
            unsigned d = (unsigned)(x % 10);
            sum += POW4[d];
            x /= 10;
        }
        f[i] = sum;
    }
}

static void compute_sequence(void)
{
    compute_f();

    for (idx_t i = 1; i <= MAXN; ++i) {
        idx_t fi = (idx_t)f[i];
        if (fi <= MAXN && f[fi] == (seq_t)i) {
            seq_results[seq_count++] = (seq_t)i;
        }
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        compute_sequence();
        initialized = true;
    }

    if (n == 0 || (idx_t)n > seq_count) {
        return 0;  /* 0 indicates out of bounds for this build */
    }
    return seq_results[(size_t)n - 1];
}
