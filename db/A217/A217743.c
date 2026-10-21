/*
 * A217743  Excess of number of odds in the form 4k+1 over number of odds in
 *          the form 4k+3 in Collatz trajectory of n.
 * https://oeis.org/A217743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = #(odd numbers ≡ 1 (mod 4) in the Collatz trajectory of n)
 *            – #(odd numbers ≡ 3 (mod 4) in the Collatz trajectory of n)
 * Range:     i8 n <= 14, i16 n <= 6464, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t cur = (seq_t)n;          /* current value in the Collatz sequence   */
    seq_t diff = 0;                /* excess of 1‑mod‑4 odds over 3‑mod‑4 odds  */

    while (1) {
        /* If cur is odd, update the excess */
        if ((cur & 1) != 0) {
            diff += (((cur & 3) == 1) ? 1 : -1);
        }

        /* Stop when the sequence reaches 1 (which is odd and 1 mod 4) */
        if (cur == (seq_t)1) {
            break;
        }

        /* Next step of the Collatz iteration */
        if ((cur & 1) == 0) {
            cur = cur / 2;         /* even case */
        } else {
            cur = cur * 3 + 1;     /* odd case */
        }
    }

    return diff;
}
