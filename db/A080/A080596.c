/*
 * A080596  a(1)=1; for n >= 2, a(n) is smallest positive integer which is
 *          consistent with sequence being monotonically increasing and
 *          satisfying a(a(n)) = 2n+3.
 * https://oeis.org/A080596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1; For n ≥ 2, let k ≥ 0 be the unique integer such that
 *            6·2^k – 3 + j = n, where –2^(k+1) ≤ j < 2^(k+1). Then a(n) =
 *            8·2^k – 3 + { 2·j if j ≥ 0 j if j < 0 }
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A080596. */
seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    /* find k by iterating power of two blocks */
    uint64_t pow2 = 1ULL;  /* 2^k */
    idx_t baseIdx = 0;
    idx_t upper   = 0;
    int   k       = 0;

    for (;; ++k)
    {
        baseIdx = 6ULL * pow2 - 3ULL;          /* 6·2^k – 3 */
        upper   = baseIdx + 2ULL * pow2 - 1ULL;/* upper bound of this block */

        if (n <= (idx_t)upper)
            break;

        /* avoid overflow, although for typical n ≤ 10000 this will never happen */
        if (pow2 > UINT64_MAX / 2ULL)
            break;      /* cannot handle larger n with this method */
        pow2 <<= 1;     /* now 2^(k+1) */
    }

    /* compute j = n – (6·2^k – 3), which may be negative */
    if (n >= (idx_t)baseIdx)
    {
        /* j ≥ 0 */
        idx_t j = n - (idx_t)baseIdx;
        uint64_t val = (8ULL << k) - 3ULL + 2ULL * j;
        return (seq_t)val;
    }
    else
    {
        /* j < 0 */
        idx_t jneg = (idx_t)baseIdx - n;          /* |j| */
        uint64_t val = (8ULL << k) - 3ULL - jneg; /* a = baseVal + j = baseVal – |j| */
        return (seq_t)val;
    }
}
