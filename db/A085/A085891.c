/*
 * A085891  Maximal product of three numbers with sum n: a(n) = max(r*s*t), n
 *          = r+s+t.
 * https://oeis.org/A085891
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Let m = n / 3. – If n % 3 == 0: a(n) = m³ – If n % 3 == 1: a(n)
 *            = m² · (m + 1) – If n % 3 == 2: a(n) = m · (m + 1)² This is the
 *            maximal product of three non‑negative integers whose sum is n
 * Range:     u8 n <= 19, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* the sequence is defined for n ≥ 3 */

    idx_t      r = n % 3;          /* n % 3 : 0, 1, or 2          */
    seq_t      m = (seq_t)(n / 3); /* floor(n/3) as seq_t          */
    seq_t      mp1 = m + 1;        /* m + 1, used in the latter two cases */

    if (r == 0) {                 /* n ≡ 0 (mod 3)   →  m³          */
        return m * m * m;
    } else if (r == 1) {          /* n ≡ 1 (mod 3)   →  m²·(m+1)     */
        return m * m * mp1;
    } else {                      /* n ≡ 2 (mod 3)   →  m·(m+1)²     */
        return m * mp1 * mp1;
    }
}
