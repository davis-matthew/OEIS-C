/*
 * A211066  Number of 2 X 2 matrices having all terms in {1,...,n} and
 *          nonnegative even determinant.
 * https://oeis.org/A211066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A211064(n) + A134506(n)) / 2 The sequence counts 2×2
 *            matrices with entries in {1,…,n} whose determinant is
 *            non‑negative and even. The computation uses the identity a(n) =
 *            (AE² + AO² + S_even + S_odd) / 2, where AE = n² - AO (pairs
 *            giving an even product) AO = ⌈n/2⌉² (pairs giving an odd p...
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 341, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------ */
/*   Memoised totient sieve up to the largest n requested          */
/* ------------------------------------------------------------ */
static idx_t *φ_array = NULL;
static idx_t φ_max = 0;

static void compute_totient(idx_t limit)
{
    if (limit <= φ_max)
        return;

    if (φ_array)
        free(φ_array);
    φ_array = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    if (!φ_array)
        return; /* allocation failed – silence */

    for (idx_t i = 0; i <= limit; ++i)
        φ_array[i] = i;

    for (idx_t p = 2; p <= limit; ++p)
    {
        if (φ_array[p] == p) /* p is prime */
        {
            for (idx_t j = p; j <= limit; j += p)
                φ_array[j] -= φ_array[j] / p;
        }
    }
    φ_max = limit;
}

/* ------------------------------------------------------------ */
/*   Main sequence function                                       */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* should not happen for this sequence */

    /* Prepare totient values up to n */
    compute_totient(n);

    /* --- Compute AO and AE ----------------------------------- */
    idx_t m = (n + 1) / 2;                     /* number of odds in [1..n] */
    seq_t AO = (seq_t)m * (seq_t)m;            /* odd‑product pairs */
    seq_t n_sq = (seq_t)n * (seq_t)n;          /* n²              */
    seq_t AE = n_sq - AO;                      /* even‑product pairs */

    /* --- Sum squares of frequencies for equal products ---- */
    seq_t count_eq_odd  = (seq_t)n * (seq_t)n;  /* k = 1 contributes n²   */
    seq_t count_eq_even = 0;

    for (idx_t k = 2; k <= n; ++k)
    {
        idx_t t = n / k;                       /* floor(n / k)       */
        seq_t t_sq = (seq_t)t * (seq_t)t;
        seq_t add = (seq_t)φ_array[k] * t_sq;
        add <<= 1; /* multiply by 2 */

        if ( (k & 1) == 0 ) /* even k */
            count_eq_even += add;
        else /* odd k */
            count_eq_odd  += add;
    }

    /* --- Final combination ----------------------------------- */
    seq_t AE2   = AE * AE;
    seq_t AO2   = AO * AO;
    seq_t total = AE2 + AO2 + count_eq_even + count_eq_odd;

    return total / 2;   /* division is exact */
}
