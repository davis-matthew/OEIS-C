/*
 * A083226  a(n) = (8*9^n + (-9)^n)/9.
 * https://oeis.org/A083226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (8*9^n + (-9)^n) / 9 Recurrence: a(0) = 1, a(1) = 7, a(n)
 *            = 81 * a(n-2) for n ≥ 2 Implemented via exponentiation: a(2k) =
 *            81^k a(2k+1) = 7 * 81^k
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fast modular‑free exponentiation for seq_t values.  Works for both
   fixed‑width integer types (uint8_t, …, unsigned __int128) and
   arbitrary‑precision integer classes that support the built‑in
   arithmetic operators. */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1U)
            result = result * base;
        base = base * base;
        exp >>= 1U;
    }
    return result;
}
 
seq_t a(idx_t n)
{
    idx_t k = n >> 1;                /* floor(n/2)  */
    seq_t res = pow_seq_t((seq_t)81, k);
    if (n & 1U)                      /* odd n */
        res = res * (seq_t)7;
    return res;
}
