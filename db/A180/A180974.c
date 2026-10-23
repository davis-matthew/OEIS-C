/*
 * A180974  Numbers n such that 10^11 + n^2 is a square.
 * https://oeis.org/A180974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 50, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A180974: numbers n such that 10^11 + n^2 is a perfect square.
 * 
 * The equation 10^11 + n^2 = y^2 leads to (y-n)(y+n) = 10^11.
 * Let a = y-n, b = y+n.  Since 10^11 is even, a and b are both even.
 * Write a = 2a', b = 2b'; then (2a')(2b') = 10^11 → a'b' = 10^11/4 = 25 000 000 000.
 * For each divisor pair a' < b' of 25 000 000 000 we obtain a solution
 *     n = b' – a'.
 * All solutions are thus differences of complementary divisors of 25 000 000 000.
 * The 60 distinct values are sorted in increasing order.
 */

#include <stdint.h>
#include <stddef.h>

static seq_t seq[60];
static size_t seq_count = 0;

static void init_seq(void)
{
    if (seq_count != 0) return;                /* already initialised */

    const uint64_t N = 25000000000ULL;          /* 10^11 / 4 */
    size_t idx = 0;

    for (uint64_t d = 1; d * d <= N; ++d) {
        if (N % d == 0) {
            uint64_t q = N / d;
            if (d < q) {
                seq[idx++] = (seq_t)(q - d);
            }
        }
    }

    /* simple insertion sort – only 60 elements */
    for (size_t i = 1; i < idx; ++i) {
        seq_t key = seq[i];
        size_t j = i;
        while (j > 0 && seq[j - 1] > key) {
            seq[j] = seq[j - 1];
            --j;
        }
        seq[j] = key;
    }

    seq_count = idx;
}

/* External linkage as required */
seq_t a(idx_t n)
{
    init_seq();
    if (n == 0 || (size_t)n > seq_count) return (seq_t)0;
    return seq[n - 1];
}
