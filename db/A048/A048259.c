/*
 * A048259  Number of distinct solutions to x + y + z = 0 (mod n), where two
 *          solutions are equivalent if one can be obtained from the other by
 *          multiplying by units in Z/nZ and permuting x,y,z.
 * https://oeis.org/A048259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(6 φ(n))) * Σ_{u ∈ U(n)} [ g(u-1)^2 + 3 ⋅
 *            (g(u-1)·g(u^2-1)⋅gcd(n,d₂,d₁)/n) + 2 ⋅ (g(u^3-1)⋅gcd(n,d₃)/n) ]
 *            where g(t) = gcd(n, t) d₁ = n/g(u-1) d₂ = (1 + u^{-1}) ⋅
 *            n/g(u^2-1) (mod n) d₃ = (1 + u^{-1} + (u^{-1})²) ⋅ n/g(u^3-1)
 *            (mod n)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: gcd for idx_t */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Helper: modular inverse of a modulo m (a and m coprime) */
static idx_t modinv(idx_t a, idx_t m)
{
    /* Extended Euclid */
    int64_t t = 0, newt = 1;
    int64_t r = m, newr = a;
    while (newr)
    {
        int64_t q = r / newr;
        int64_t tmp = newt;          newt = t - q * newt;  t = tmp;
        tmp = newr;                  newr = r - q * newr;  r = tmp;
    }
    /* r is gcd, should be 1 */
    if (r != 1) return 0;          /* should not happen for unit u */
    if (t < 0) t += m;
    return (idx_t)t;
}

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return (seq_t)1;

    idx_t orig_n = n;
    /* compute φ(n) */
    idx_t temp = orig_n;
    idx_t phi = orig_n;
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            phi = phi / p * (p - 1);
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) phi = phi / temp * (temp - 1);

    unsigned __int128 sum = 0;

    for (idx_t u = 1; u < orig_n; ++u)
    {
        if (gcd_idx(u, orig_n) != 1) continue;   /* unit only */

        /* ---------- identity permutation ---------- */
        idx_t g_id = gcd_idx(orig_n, u - 1);
        unsigned __int128 N_id = (unsigned __int128)g_id * g_id;
        sum += N_id;

        /* ---------- transposition permutations (3 of them) ---------- */
        /* cycle lengths 2 and 1 */
        idx_t u2 = (u * u) % orig_n;
        idx_t g1 = g_id;                       /* gcd(u-1, n) */
        idx_t g2 = gcd_idx(orig_n, (u2 + orig_n - 1) % orig_n); /* gcd(u^2-1, n) */
        idx_t r1 = orig_n / g1;                /* n/g1 */
        idx_t r2 = orig_n / g2;                /* n/g2 */

        idx_t u_inv = modinv(u, orig_n);
        idx_t S2 = (1 + u_inv) % orig_n;

        idx_t d2 = ((idx_t)S2 * r2) % orig_n;
        idx_t d1 = r1 % orig_n;
        idx_t g_trans = gcd_idx(orig_n, gcd_idx(d1, d2));

        unsigned __int128 N_trans = ((unsigned __int128)g2 * g1 * g_trans) / orig_n;
        sum += (unsigned __int128)3 * N_trans;

        /* ---------- 3‑cycle permutations (2 of them) ---------- */
        idx_t u3 = (u2 * u) % orig_n;
        idx_t pow3_minus1 = (u3 + orig_n - 1) % orig_n; /* u^3-1 (mod n) */
        idx_t g3 = gcd_idx(orig_n, pow3_minus1);
        idx_t r3 = orig_n / g3;

        idx_t u_inv2 = (u_inv * u_inv) % orig_n;
        idx_t S3 = (1 + u_inv + u_inv2) % orig_n;

        idx_t d3 = ((idx_t)S3 * r3) % orig_n;
        idx_t g_cycle = gcd_idx(orig_n, d3);

        unsigned __int128 N_cycle3 = ((unsigned __int128)g3 * g_cycle) / orig_n;
        sum += (unsigned __int128)2 * N_cycle3;
    }

    unsigned __int128 denom = (unsigned __int128)6 * phi;
    unsigned __int128 result = sum / denom;

    return (seq_t)result;
}
