/*
 * A073059  a(n) = (1/2)*(A073504(n+1) - A073504(n)).
 * https://oeis.org/A073059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9719, u16 n <= 9719, u32 n <= 9719, u64 n <= 9719,
 *            u128 n <= 9719, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A073059
 * Definition: a(n) has the recurrence
 *  a(2n)   = 0
 *  a(4n+3) = 1
 *  a(4n+1) = a(n)
 * with a(1)=0, a(2)=0, a(3)=1.
 *
 * This implementation evaluates the recurrence by iteratively
 * reducing n until a terminal residue (0,2,3) modulo 4 is
 * encountered.  Complexity: O(log n). */
seq_t a(idx_t n)
{
    while (1) {
        switch (n & 3) {           /* n mod 4 */
            case 0:                /* n ≡ 0 (mod 4) ⇒ a(n)=0 */
            case 2:                /* n ≡ 2 (mod 4) ⇒ a(n)=0 */
                return (seq_t)0;
            case 3:                /* n ≡ 3 (mod 4) ⇒ a(n)=1 */
                return (seq_t)1;
            case 1:                /* n ≡ 1 (mod 4) ⇒ a(n) = a(n/4) */
                n >>= 2;            /* divide by 4 */
                continue;
        }
    }
}
