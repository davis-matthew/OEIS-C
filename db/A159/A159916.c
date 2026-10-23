/*
 * A159916  Triangle T(m,n) = number of subsets of {1,...,m} with n elements
 *          having an odd sum, 1 <= n <= m.
 * https://oeis.org/A159916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 198, u32 n <= 646, u64 n <= 2376,
 *            u128 n <= 8841, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A159916:
   a(n) = number of subsets of {1,…,m} of size k with an odd sum
   where m is the unique integer such that  m(m+1)/2 ≥ n,
   k = n – m(m–1)/2.
   For given m,k:  let e = floor(m/2), o = m – e.
   T(m,k) = Σ_{t odd, 0≤t≤min(k,o)} C(o,t)·C(e,k–t).  */

/* compute binomial coefficient C(n,k) for unsigned 64‑bit arguments */
static inline seq_t binom_uint64(unsigned long long n, unsigned long long k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (unsigned long long i = 1; i <= k; ++i)
        r = r * (n - k + i) / i;
    return r;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* find m such that m(m+1)/2 >= n */
    idx_t m = 1;
    while ( (m * (m + 1)) / 2 < n ) ++m;

    /* k = offset within row m (1‑based) */
    idx_t before = ((m - 1) * m) / 2;
    idx_t k = n - before;          /* 1 ≤ k ≤ m */

    /* even and odd counts in {1,…,m} */
    int e = (int)(m / 2);          /* floor(m/2) */
    int o = (int)m - e;            /* ceil(m/2) */

    int k_i = (int)k;
    int max_t = k_i < o ? k_i : o;  /* maximum number of odds that can be chosen */

    seq_t total = 0;
    for (int t = 1; t <= max_t; t += 2)      /* t odd only */
    {
        int e_needed = k_i - t;              /* evens to choose */
        if (e_needed < 0 || e_needed > e) continue;
        seq_t term = binom_uint64((unsigned long long)o, (unsigned long long)t);
        term = term * binom_uint64((unsigned long long)e, (unsigned long long)e_needed);
        total += term;
    }
    return total;
}
