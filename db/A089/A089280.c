/*
 * A089280  Tower of Hanoi game: a(n) is the number of pegs occupied by
 *          already-moved disks after move #n.
 * https://oeis.org/A089280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): For n ≥ 1, let the binary representation of n be b_k … b_0.
   Split it into maximal runs of identical bits.
   If there is only one run → a(n) = 1.
   Otherwise ignore the first and last run.
   If any of the remaining runs has odd length → a(n) = 3,
   otherwise → a(n) = 2. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥ 1 */
    uint64_t x = n;                /* simple 64‑bit copy of the index   */
    int bit_len = 0;               /* number of bits in x (≥ 1)         */

    /* compute bit length of n without a helper function */
    uint64_t tmp = x;
    while (tmp) {
        ++bit_len;
        tmp >>= 1;
    }

    /* collect run lengths of consecutive equal bits */
    uint8_t runs[65];
    int cnt = 0;                /* number of runs found             */

    uint8_t cur_bit = (x >> (bit_len - 1)) & 1ULL;  /* MSB                     */
    int run_len = 1;                     /* length of the current run    */

    for (int i = bit_len - 2; i >= 0; --i) {
        uint8_t bit = (x >> i) & 1ULL;
        if (bit == cur_bit) {
            ++run_len;
        } else {
            runs[cnt++] = (uint8_t)run_len;
            cur_bit = bit;
            run_len = 1;
        }
    }
    runs[cnt++] = (uint8_t)run_len;   /* last run */

    /* apply the rule described above */
    if (cnt == 1)                     /* only one run → answer is 1 */
        return (seq_t)1;

    bool odd_found = false;
    for (int i = 1; i <= cnt - 2; ++i) { /* ignore first and last run */
        if (runs[i] & 1U) {            /* odd length */
            odd_found = true;
            break;
        }
    }
    return odd_found ? (seq_t)3 : (seq_t)2;
}
