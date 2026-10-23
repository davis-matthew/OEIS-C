/*
 * A173726  Number of reduced, normalized 3 X 3 semimagic squares with magic
 *          sum n.
 * https://oeis.org/A173726
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3-24*n^2+176*n-264)/48 + (n mod 2)*(n^2-21*n+55)/16 +
 *            ((2*n^2+1) mod 3)*(n-9)/3 + ((n^3+n^2+n+2) mod 4)*n/8 - [(n mod
 *            4)=0]*7/2 - [(n mod 5)=0]*2 - [(n mod 6)=0]*2 - [(n mod 7)=0] -
 *            [(n mod 8)=0] The implementation evaluates a common‑denominator
 *            expression (dividing by 48 at the end) to...
 * Range:     u8 n <= 30, u16 n <= 154, u32 n <= 5914, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For indices below the offset (12) the value is irrelevant. */
    if (n < 12)
        return (seq_t)0;

    /* Use 64-bit signed arithmetic for intermediate calculations. */
    int64_t nn = (int64_t)n;
    int64_t n2 = nn * nn;
    int64_t n3 = n2 * nn;

    /* Begin with the main polynomial term. */
    int64_t tot = n3 - 24 * n2 + 176 * nn - 264;          /* /48 */

    /* (n mod 2)*(n^2-21*n+55)/16  → multiply by 3 (/48). */
    if ((nn & 1LL) != 0) {
        tot += 3 * (n2 - 21 * nn + 55);
    }

    /* ((2*n^2+1) mod 3)*(n-9)/3  → multiply by 16 (/48). */
    int64_t mod3 = (2 * n2 + 1) % 3;
    if (mod3 != 0) {
        tot += 16 * mod3 * (nn - 9);
    }

    /* ((n^3+n^2+n+2) mod 4)*n/8  → multiply by 6 (/48). */
    int64_t mod4 = (n3 + n2 + nn + 2) % 4;
    if (mod4 != 0) {
        tot += 6 * mod4 * nn;
    }

    /* Subtraction terms, multiplied to common denominator 48. */
    if (nn % 4 == 0) {
        tot -= 168;   /* 7/2 * 48 = 168 */
    }
    if (nn % 5 == 0) {
        tot -= 96;    /* 2   * 48 = 96  */
    }
    if (nn % 6 == 0) {
        tot -= 96;    /* 2   * 48 = 96  */
    }
    if (nn % 7 == 0) {
        tot -= 48;    /* 1   * 48 = 48  */
    }
    if (nn % 8 == 0) {
        tot -= 48;    /* 1   * 48 = 48  */
    }

    /* Final division by 48 gives the integer sequence value. */
    return (seq_t)(tot / 48);
}
