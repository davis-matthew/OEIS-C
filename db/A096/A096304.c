/*
 * A096304  Numbers k such that 3k does not divide (6k-4)!/((3k-2)!*(3k-1)!).
 * https://oeis.org/A096304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9 * A005836(floor(n/6)) + (n mod 6)
 * Range:     u8 n <= 57, u16 n <= 600, u32 n <= 600, u64 n <= 600,
 *            u128 n <= 600, big any n
 * Validated: standard, pass, 77 terms through n = 600 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A005836_idx(idx_t x)
{
    seq_t result = 0;
    seq_t pow3   = 1;      /* 3^0 */
    while (x) {
        if (x & 1U) result += pow3;
        pow3 *= (seq_t)3;
        x >>= 1U;
    }
    return result;
}
/* A005836(x) = numbers whose base‑3 representation contains only digits 0 or 1 */
/* (equivalently, interpret the binary digits of x as base‑3 digits). */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined for n < 1, return 0 */
    idx_t  k   = n / 6;                    /* floor(n/6) */
    seq_t  val = A005836_idx(k);            /* A005836(k) */
    seq_t  mod = (seq_t)(n % 6);            /* (n mod 6) */
    return 9 * val + mod;
}
