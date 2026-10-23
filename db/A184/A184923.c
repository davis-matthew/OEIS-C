/*
 * A184923  a(n) = n+[r*n/u]+[s*n/u]+[t*n/u], where []=floor and r=2^(1/2),
 *          s=r+1, t=r+2, u=r+3.
 * https://oeis.org/A184923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊(3√2-2)·n/7⌋ + ⌊(1+2√2)·n/7⌋ + ⌊(4+√2)·n/7⌋
 * Range:     u8 n <= 96, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A184923 - partition of the positive integers (offset 1) */
seq_t a(idx_t n)
{
    /* Pre‑computed integer numerators for the three Beatty terms,
       derived from the rational approximation √2 ≈ 14142135/10⁷. */
    const unsigned long long N0 = 22426405ULL;   /* (-2·10⁷ + 3·14142135) */
    const unsigned long long N1 = 38284270ULL;   /* (1·10⁷ + 2·14142135) */
    const unsigned long long N2 = 54142135ULL;   /* (4·10⁷ + 1·14142135) */
    const unsigned long long DEN = 70000000ULL;  /* 7·10⁷ */

    /* Compute each floor term with 128‑bit arithmetic to avoid overflow. */
    seq_t t0 = (seq_t)(( (unsigned __int128)n * N0 ) / DEN);
    seq_t t1 = (seq_t)(( (unsigned __int128)n * N1 ) / DEN);
    seq_t t2 = (seq_t)(( (unsigned __int128)n * N2 ) / DEN);

    /* Combine with the linear part. */
    return (seq_t)n + t0 + t1 + t2;
}
