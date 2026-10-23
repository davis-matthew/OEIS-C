/*
 * A133623  Binomial(n+p, n) mod n where p=3.
 * https://oeis.org/A133623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+3,3) mod n Simplified piecewise formula for n
 *            > 6: if n % 6 == 0 : a = 5*n/6 + 1 if n % 6 == 1 or 5 : a = 1 if
 *            n % 6 == 2 or 4 : a = n/2 + 1 if n % 6 == 3 : a = n/3 + 1 Small
 *            n values (n <= 6) are hard‑coded to match the OEIS b‑file
 * Range:     u8 n <= 305, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first 6 entries explicitly */
    if (n <= 6) {
        switch ((int)n) {
            case 1: return (seq_t)0;
            case 2: return (seq_t)0;
            case 3: return (seq_t)2;
            case 4: return (seq_t)3;
            case 5: return (seq_t)1;
            case 6: return (seq_t)0;
            default: return (seq_t)0; /* unreachable for n >= 1 */
        }
    }

    /* n > 6 */
    const idx_t mod = n % 6;

    if (mod == 0) {                     /* n divisible by 6 */
        return (seq_t)(5 * (n / 6) + 1);
    } else if (mod == 1 || mod == 5) {   /* n ≡ 1 or 5 (mod 6) */
        return (seq_t)1;
    } else if (mod == 2 || mod == 4) {   /* n even but not divisible by 6 */
        return (seq_t)(n / 2 + 1);
    } else { /* mod == 3, n divisible by 3 */
        return (seq_t)(n / 3 + 1);
    }
}
