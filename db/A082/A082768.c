/*
 * A082768  Numbers that begin with 1, 3, 7 or 9.
 * https://oeis.org/A082768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A082768 – Numbers that begin with 1, 3, 7 or 9. */
/* a(n) = the nth positive integer whose leading decimal digit ∈ {1,3,7,9}. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* Undefined for n=0, but guard anyway. */
    idx_t count = 0;
    seq_t num = 1;
    for (;; ++num) {
        /* Compute the most significant decimal digit of num. */
        seq_t x = num;
        while (x >= 10) {
            x /= 10;
        }
        /* Check if the first digit is one of the allowed digits. */
        if (x == 1 || x == 3 || x == 7 || x == 9) {
            ++count;
            if (count == n) return num;
        }
    }
}
