/*
 * A008952  Leading digit of 2^n.
 * https://oeis.org/A008952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(10^{fractional part of n*log10(2)})
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 9986,
 *            u128 n <= 9986, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Leading digit of 2^n, computed without floating point arithmetic */

/* scaled log10(2) as a 128‑bit integer: 0.3010299956639811952137389… *
 * 10^18  ≈ 301029995663981195LL  (floor of the real value) */
static const unsigned __int128 LOG10_2_NUM = 301029995663981195ULL;
static const uint64_t LOG10_DEN = 1000000000000000000ULL;

/* thresholds for log10(d+1) * 10^18, d=1..9 */
static const uint64_t THRESH[10] = {
    0ULL,                           /* unused */
    301029995663981195ULL,          /* log10(2)   */
    477121254719662437ULL,          /* log10(3)   */
    602059991327962390ULL,          /* log10(4)   */
    698970004336018804ULL,          /* log10(5)   */
    778151250383642632ULL,          /* log10(6)   */
    845098040014256830ULL,          /* log10(7)   */
    903089987221922392ULL,          /* log10(8)   */
    954242509439324875ULL,          /* log10(9)   */
    1000000000000000000ULL          /* log10(10)  */
};

/* Compute leading digit of 2^n.  For small n we compute exact 2^n
   and extract the most significant digit; for larger n we use the
   scaled logarithmic method described above. */
seq_t a(idx_t n)
{
    /* Handle small n exactly (to avoid rounding errors on very small
       powers where 2^n is an exact power of ten times an integer). */
    if (n < 64) {
        seq_t val = 1;
        for (idx_t i = 0; i < n; ++i)
            val *= 2;
        while (val >= 10)
            val /= 10;
        return val;
    }

    /* Compute n*log10(2) modulo 1 using integer arithmetic. */
    unsigned __int128 prod = (unsigned __int128)n * LOG10_2_NUM;
    uint64_t fractional = (uint64_t)(prod % LOG10_DEN); /* scaled by 10^18 */

    /* Determine the leading digit by comparing with thresholds. */
    for (int d = 1; d <= 9; ++d) {
        if (fractional < THRESH[d])
            return (seq_t)d;
    }
    return (seq_t)9; /* Fallback (should never happen) */
}
