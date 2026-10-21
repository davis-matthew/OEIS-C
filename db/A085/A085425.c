/*
 * A085425  Number of minus ones in the symmetric signed digit expansion of n
 *          with q=2 (i.e., the representation of n in the (-1,0,1)_2 number
 *          system).
 * https://oeis.org/A085425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   number of minus ones in the symmetric signed‑digit expansion of
 *            n using digits -1,0,1 and base 2 (non‑adjacent form)
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t m = n;
    while (m) {
        if (m & 1ULL) {                    // odd
            if ((m & 3ULL) == 3ULL) {      // n mod 4 == 3 → digit = -1
                count++;
                m += 1ULL;                // add carry before shifting
            }
            m >>= 1;                       // divide by 2
        } else {
            m >>= 1;                       // even → just shift
        }
    }
    return count;
}
