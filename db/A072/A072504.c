/*
 * A072504  a(n) = LCM of divisors of n which are <= sqrt(n).
 * https://oeis.org/A072504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm of all divisors d of n with d <= sqrt(n)
 * Range:     u8 n <= 259, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the definition directly with integer arithmetic.
   Uses only standard integer operators and no external helper functions. */

static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    /* Euclidean algorithm – works for any seq_t type */
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static seq_t lcm_seq_t(seq_t a, seq_t b)
{
    if (a == (seq_t)0 || b == (seq_t)0) return (seq_t)0;
    seq_t g = gcd_seq_t(a, b);
    return (a / g) * b;
}

/* Integer square root using binary search – returns floor(sqrt(n)) */
static idx_t int_sqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;      /* out‑of‑range safety */

    idx_t sqrt_n = int_sqrt(n);
    seq_t lcm_val = (seq_t)1;

    for (idx_t d = (idx_t)1; d <= sqrt_n; ++d) {
        if (n % d == (idx_t)0) {
            lcm_val = lcm_seq_t(lcm_val, (seq_t)d);
        }
    }
    return lcm_val;
}
