/*
 * A204697  Final nonzero digit of n^n in base 3.
 * https://oeis.org/A204697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   final non‑zero digit of n^n in base 3 a(0) = 1 For n>0: let v =
 *            v₃(n) // exponent of 3 in n let base = n / 3^v // n with all
 *            3‑factors removed If base mod 3 = 1 → result 1 If base mod 3 = 2
 *            → result = (n odd ? 2 : 1) // because 2ⁿ mod 3 (Indices start at
 *            0 as in OEIS.)
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t temp = n;
    while (temp % 3 == 0)
        temp /= 3;  /* temp now n / 3^v₃(n), but not used further */

    idx_t base = n;
    while (base % 3 == 0)
        base /= 3;  /* remove all factors of 3 */

    idx_t bmod = base % 3;
    if (bmod == 1)
        return (seq_t)1;          /* base ≡ 1 (mod 3) → 1ⁿ ≡ 1 */

    /* bmod == 2  → compute 2ⁿ mod 3 */
    if ((n & 1u) == 1u)
        return (seq_t)2;          /* n odd → 2 */
    return (seq_t)1;              /* n even → 1 */
}
