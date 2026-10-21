/*
 * A078173  a(n) = max(min(r,s,t)), where the maximum is taken over all
 *          positive integer solutions of the Diophantine equation r*s*t = n *
 *          (r+s+t).
 * https://oeis.org/A078173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max { min(r,s,t) | r*s*t = n*(r+s+t) } (r,s,t positive
 *            integers)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  this implementation searches for the maximum r such that there exist s,t ≥ r
    with r*s*t = n*(r+s+t). It iterates r from 3*n downwards and, for each r,
    finds an s that yields an integer t ≥ s.  The search stops as soon as a valid
    triple is found, guaranteeing an O(n) algorithm for n up to about 10 000. */

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for 0; return 0 to keep function well‑defined */
        return (seq_t)0;

    /* Upper bound for r is 3*n because 1/r + 1/r + 1/r ≥ 1/n ⇒ r ≤ 3*n. */
    idx_t r_max = 3 * n;

    for (idx_t r = r_max; r > 0; --r)
    {
        /* Try all s ≥ r.  The loop terminates as soon as t < s, since
           t decreases monotonically with s. */
        idx_t s = r;
        while (1)
        {
            /* Compute r*s and check against n to avoid negative denominators. */
            unsigned __int128 prod = (unsigned __int128)r * s;
            if (prod <= (unsigned __int128)n)   /* denominator ≤ 0 */
            {
                ++s;
                continue;
            }

            unsigned __int128 denom = prod - (unsigned __int128)n;
            unsigned __int128 num = (unsigned __int128)n * (r + s);

            /* Check divisibility: t must be integer. */
            if (num % denom == 0)
            {
                unsigned __int128 t = num / denom;
                if (t >= (unsigned __int128)s)   /* t ≥ s ensures r ≤ s ≤ t */
                    return (seq_t)r;             /* found maximal r */
            }

            /* If even the floor of t is less than s, further s will only
               decrease t, so no solution exists for this r. */
            unsigned __int128 t_floor = num / denom;
            if (t_floor < (unsigned __int128)s)
                break;

            ++s;
        }
    }

    /* Fallback – by construction r = 1 always works. */
    return (seq_t)1;
}
