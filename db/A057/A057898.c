/*
 * A057898  Largest number such that n = m^a(n) - a(n) with m a positive
 *          integer; i.e., where (n + a(n))^(1/a(n)) is a positive integer.
 * https://oeis.org/A057898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest k such that n + k is a k‑th power of an integer
 *            (equivalently, ∃m∈ℕ : n = m^k – k). The implementation searches
 *            exponents k from an upper bound down to 1 and checks for an
 *            exact k‑th root
 * Range:     u8 n <= 1313, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*-----------------------------------------------------------*/
/*  Integer power with overflow guard – returns
    limit+1 if the intermediate would exceed 'limit'
    (so the result never overflows seq_t).            */
/*-----------------------------------------------------------*/
static seq_t pow_limit(seq_t base, idx_t exp, seq_t limit)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i) {
        if (result > limit / base)       /* would overflow limit */
            return limit + 1;            /* sentinel greater than limit */
        result *= base;
    }
    return result;
}

/*-----------------------------------------------------------*/
/*  Floor of log₂ for an unsigned integer index value.     */
/*-----------------------------------------------------------*/
static idx_t floor_log2_idx(idx_t x)
{
    idx_t r = 0;
    while (x >>= 1) ++r;
    return r;
}

/*-----------------------------------------------------------*/
/*  Return the exact integer k‑th root of target if it
    exists; otherwise return 0.  Uses a bounded binary search
    with pow_limit for overflow protection.                */
/*-----------------------------------------------------------*/
static seq_t k_th_root_exact(seq_t target, idx_t k)
{
    /* Find an upper bound for the root by doubling. */
    idx_t low  = 1;
    idx_t high = 1;
    while (pow_limit((seq_t)high, k, target) <= target)
        high <<= 1;

    /* Binary search for the exact root. */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        seq_t val = pow_limit((seq_t)mid, k, target);
        if (val <= target)
            low = mid;
        else
            high = mid - 1;
    }

    seq_t val = pow_limit((seq_t)low, k, target);
    return (val == target) ? low : 0;
}

/*-----------------------------------------------------------*/
/*  Main sequence function.                                 */
/*-----------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)               /* sequence is defined for n ≥ 1 */
        return 0;

    /* Upper bound for k: 2^k ≤ 2n  ⇒  k ≤ ⌊log₂(2n)⌋ = ⌊log₂ n⌋ + 1   */
    idx_t max_k = floor_log2_idx(n) + 1;

    for (idx_t k = max_k; k >= 1; --k) {
        seq_t target = (seq_t)n + (seq_t)k;   /* n + k must be a k‑th power */
        if (k_th_root_exact(target, k))
            return (seq_t)k;                 /* largest admissible k found */
        if (k == 1) break;                   /* avoid underflow of unsigned */
    }
    return 1;                                 /* k = 1 always works */
}
