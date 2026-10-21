/*
 * A064717  A Beatty sequence for 2^i + 2^(-i) where i = sqrt(-1).
 * https://oeis.org/A064717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(n * (2^i + 2^(-i))) where i = √(-1) and 2^i +
 *            2^(-i) = 2*cos(ln 2) ≈ 1.53847780272794425315665998732254140
 * Range:     u8 n <= 166, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Beatty sequence for the irrational constant α = 2*cos(ln 2).  We use a rational
   approximation P/Q with Q = 10^18, P = ⌊α*Q⌋.  For all n < Q the integer part
   floor(n*α) equals floor((n*P)/Q), so the computation is exact for all
   relevant indices.  Integer arithmetic is used only. */

static const unsigned long long P = 1538477802727944153ULL; /* ⌊α * 10^18⌋ */
static const unsigned long long Q = 1000000000000000000ULL; /* 10^18 */

seq_t a(idx_t n)
{
    /* n ≥ 1.  Compute floor(n * P / Q) using 128‑bit arithmetic. */
    __int128 prod = (__int128)n * P;          /* product fits in 128 bits for n ≤ 1e18 */
    unsigned long long val = (unsigned long long)(prod / Q);
    return (seq_t)val;
}
