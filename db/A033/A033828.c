/*
 * A033828  Numbers that can be expressed as the product of two 2-digit
 *          numbers in at least one way.
 * https://oeis.org/A033828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the nth smallest integer that can be expressed as a
 *            product of two 2‑digit numbers (10–99). The sequence is finite;
 *            after the last term (9801) this routine returns 0. The
 *            implementation builds a static table on the first call and then
 *            indexes it directly
 * Range:     u8 n <= 48, u16 n <= 2621, u32 n <= 2621, u64 n <= 2621,
 *            u128 n <= 2621, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static bool initialized = false;

/* Largest possible product 99*99 = 9801, smallest 10*10 = 100 */
#define MAX_VAL 9801
#define MIN_VAL 100
#define MAX_AB 9900      /* array size for presence lookup (index up to 9801) */

/* Storage for the sorted unique products */
static seq_t seq[10000];    /* 10000 is safe > 2621 unique terms */
static size_t seq_len = 0;

/* Helper to build the sequence once */
static void init_sequence(void)
{
    if (initialized) return;

    bool present[MAX_AB + 1] = {0};

    /* Mark all products of two 2‑digit numbers */
    for (idx_t a = 10; a <= 99; ++a) {
        for (idx_t b = 10; b <= 99; ++b) {
            present[a * b] = true;
        }
    }

    /* Extract sorted unique values into seq[] */
    for (idx_t val = MIN_VAL; val <= MAX_VAL; ++val) {
        if (present[val]) {
            seq[seq_len++] = (seq_t)val;
        }
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    init_sequence();

    if (n == 0 || n > seq_len) {
        /* out of range: sequence stops at 2621 terms */
        return (seq_t)0;
    }
    return seq[n - 1];
}
