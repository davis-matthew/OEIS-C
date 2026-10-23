/*
 * A033722  Expansion of Product_{d | 10} theta_3(q^d).
 * https://oeis.org/A033722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient of q^n in ∏_{d|10} θ₃(q^d),
 * where θ₃(q) = 1 + 2∑_{k≥1} q^{k²}.
 *
 * The algorithm convolves the four sparse series corresponding to the
 * divisors 1, 2, 5, 10.  Each factor contributes 1 for k = 0 and 2
 * for k > 0.
 */

#include <stdlib.h>

#if defined(__cplusplus)
static inline seq_t* allocate_seq_array(size_t n)
{ return new seq_t[n](); }
static inline void deallocate_seq_array(seq_t* p)
{ delete[] p; }
#else
static inline seq_t* allocate_seq_array(size_t n)
{ return (seq_t*)calloc(n, sizeof(seq_t)); }
static inline void deallocate_seq_array(seq_t* p)
{ free(p); }
#endif

seq_t a(idx_t n)
{
    size_t N = (size_t)n + 1;          /* array size = n+1          */
    seq_t *curr = allocate_seq_array(N);
    if (!curr) return 0;
    curr[0] = 1;                       /* product of zero factors    */

    const idx_t divisors[] = {1, 2, 5, 10};
    const size_t num_div = sizeof(divisors) / sizeof(divisors[0]);

    for (size_t di = 0; di < num_div; ++di)
    {
        idx_t d = divisors[di];
        seq_t *next = allocate_seq_array(N);
        if (!next) { deallocate_seq_array(curr); return 0; }

        for (idx_t s = 0; s < (idx_t)N; ++s)
        {
            seq_t val = curr[s];
            if (val == 0) continue;

            /* iterate over all k such that d·k² ≤ n – s          */
            for (idx_t k = 0; ; ++k)
            {
                idx_t k2   = k * k;
                idx_t add  = d * k2;
                if (add > n - s) break;

                idx_t idx = s + add;                /* target index   */
                seq_t weight = (k == 0) ? (seq_t)1 : (seq_t)2;
                next[idx] += val * weight;
            }
        }

        deallocate_seq_array(curr);
        curr = next;
    }

    seq_t result = curr[n];
    deallocate_seq_array(curr);
    return result;
}
