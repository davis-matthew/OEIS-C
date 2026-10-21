/*
 * A076228  Number of terms k in the trajectory of the Collatz function
 *          applied to n such that k < n.
 * https://oeis.org/A076228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: iterate Collatz steps and count terms lower than the starting value */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for index 0 */
    if (n == 1) return (seq_t)0;          /* first term listed above */
    seq_t count = 0;
    unsigned long long val = (unsigned long long)n;  /* local working value */
    while (val > 1) {
        val = (val % 2 == 0) ? (val / 2) : (3 * val + 1);
        if (val < n) count++;
    }
    return count;
}
