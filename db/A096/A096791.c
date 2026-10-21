/*
 * A096791  Number of partitions of n into distinct parts with even number of
 *          even parts.
 * https://oeis.org/A096791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the number of partitions of n into distinct parts with an
 *            even number of even parts. Implementation: dynamic programming
 *            over parts. The array even_arr[n] holds the desired count. The
 *            DP uses only + and integral arithmetic, so it works for any
 *            seq_t (unsigned integer or arbitrary‑precision...
 * Range:     u8 n <= 33, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t even_arr[10001U + 1];
static seq_t odd_arr[10001U + 1];
static bool initialized = false;

seq_t a(idx_t n)
{
    /* Limit handled by the problem definition (≈10 000).  If n exceeds
       the pre‑computed range the function returns 0 (undefined). */
    if (n > 10001U) return 0;

    if (!initialized) {
        /* Base case */
        even_arr[0] = 1U;
        initialized = true;

        /* DP: iterate over all possible parts k = 1 .. 10001 */
        for (idx_t k = 1; k <= 10001U; ++k) {
            bool even_part = ((k & 1U) == 0U);
            for (idx_t s = 10001U; s >= k; --s) {
                if (even_part) {
                    even_arr[s] += odd_arr[s - k];
                    odd_arr[s] += even_arr[s - k];
                } else {
                    even_arr[s] += even_arr[s - k];
                    odd_arr[s] += odd_arr[s - k];
                }
                if (s == k) break;   /* stop before unsigned wrap */
            }
        }
    }
    return even_arr[n];
}
