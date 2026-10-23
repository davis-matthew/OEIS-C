/*
 * A037905  a(n) = 9 - (floor(n*Pi) mod 9).
 * https://oeis.org/A037905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9 - (floor(n * Pi) mod 9)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const unsigned long long PI_NUM = 3141592653589793238ULL; /* Pi ≈ 3.141592653589793238 */
static const unsigned long long PI_DEN = 1000000000000000000ULL; /* 10^18 */

seq_t a(idx_t n)
{
    /* Compute floor(n * Pi) using fixed‑point arithmetic.
       PI = PI_NUM / PI_DEN, with an error < 1e-18; for n up to 10 000
       this guarantees the correct integer part. */
    unsigned long long n_ll = (unsigned long long)(n);
    unsigned __int128 prod = (unsigned __int128)n_ll * (unsigned __int128)PI_NUM;
    unsigned long long flo = (unsigned long long)(prod / PI_DEN);

    /* Modulo 9 and final formula. */
    unsigned long long mod9 = flo % 9ULL;
    seq_t result = (seq_t)(9ULL - mod9);   /* result is 1–9 */

    return result;
}
