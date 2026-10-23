/*
 * A183763  Number of 2 X 2 nonsingular 0..n matrices with a(1,1) <= a(1,2) <=
 *          a(2,1) <= a(2,2).
 * https://oeis.org/A183763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n+4,4) – S(n) where S(n) is the number of weakly
 *            increasing 4‑tuples (a,b,c,d) with 0≤a≤b≤c≤d≤n satisfying ad =
 *            bc. The formula above is derived from the observation that the
 *            total number of weakly increasing 4‑tuples is C(n+4,4)
 * Range:     u8 n <= 6, u16 n <= 33, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* basic gcd for idx_t (assumes idx_t is an integer type) */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* total weakly increasing 4‑tuples: C(n+4,4) */
    seq_t total = 1;
    for (idx_t i = 1; i <= 4; ++i)
        total = total * (seq_t)(n + 5 - i) / (seq_t)i; /* divisibility guaranteed */

    /* singular matrices */
    /* all matrices with a=0 give C(n+2,2) singular ones */
    seq_t singular = (seq_t)(n + 2) * (seq_t)(n + 1) / 2;

    for (idx_t a_val = 1; a_val <= n; ++a_val) {
        for (idx_t b_val = a_val; b_val <= n; ++b_val) {
            idx_t g = gcd_idx(a_val, b_val);
            idx_t a_prime = a_val / g;    /* a' = a / gcd(a,b) */
            idx_t b_prime = b_val / g;    /* b' = b / gcd(a,b) */

            /* k must satisfy  k >= ceil(b / a')  and  k <= floor(n / b') */
            idx_t k_min = (b_val + a_prime - 1) / a_prime; /* ceil division */
            idx_t k_max = n / b_prime;

            if (k_min <= k_max)
                singular += (seq_t)(k_max - k_min + 1);
        }
    }

    return total - singular;
}
