/*
 * A076366  Number of numbers for which the count of nonprimes (i.e., 1 and
 *          composites) in their reduced residue set equals n.
 * https://oeis.org/A076366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Compute A076366(n) by counting, for every x ≥ 1, the number
 *            A048864(x) of non‑prime elements (1 and composites) in the
 *            reduced residue set modulo x. Then a(n) is the cardinality of {x
 *            : A048864(x) = n}. A048864(x) = φ(x) − π(x−1) + ω(x) if x is
 *            composite, A048864(x) = φ(x) − π(x−1) if x is prime,...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Adjust these limits if larger ranges are required.  The defaults
    comfortably cover the first 10 000 terms and run in < 2 s.  */
#define MAX_X 2000000          /* upper bound for x to examine */
#define MAX_N 200000           /* storage for a(1)..a(MAX_N)           */
/* ------------------------------------------------------------------ */

static seq_t *count_arr = NULL;
static bool initialized = false;

/* ------------------------------------------------------------------ */
/* Pre‑compute φ, π and ω for 1 … MAX_X, then fill count_arr. */
static void precompute(void)
{
    /* arrays of size MAX_X+1 */
    int  *phi    = (int *)malloc((MAX_X + 1) * sizeof(int));
    int  *pi     = (int *)malloc((MAX_X + 1) * sizeof(int));
    int  *omega  = (int *)malloc((MAX_X + 1) * sizeof(int));
    bool *is_comp= (bool *)calloc(MAX_X + 1, sizeof(bool));

    /* small prime sieve */
    int  *primes = (int *)malloc((MAX_X / 10 + 1) * sizeof(int));
    int   primes_cnt = 0;

    /* base cases */
    phi[0]   = 0;
    phi[1]   = 1;
    pi[0]    = 0;
    pi[1]    = 0;
    omega[0] = 0;
    omega[1] = 0;

    /* linear sieve for φ, ω and composite flags */
    for (int i = 2; i <= MAX_X; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = i;
            phi[i]   = i - 1;
            omega[i] = 1;
        }
        for (int j = 0; j < primes_cnt; ++j) {
            int p   = primes[j];
            long long v = (long long)i * p;
            if (v > MAX_X) break;
            is_comp[v] = true;
            if (i % p == 0) {
                phi[v]   = phi[i] * p;
                omega[v] = omega[i];
                break;
            } else {
                phi[v]   = phi[i] * (p - 1);
                omega[v] = omega[i] + 1;
            }
        }
    }

    /* build π array: number of primes ≤ i */
    int pcnt = 0;
    for (int i = 0; i <= MAX_X; ++i) {
        if (i >= 2 && !is_comp[i]) ++pcnt;
        pi[i] = pcnt;
    }

    /* allocate counter array and zero it */
    count_arr = (seq_t *)calloc((size_t)MAX_N + 1, sizeof(seq_t));

    /* iterate over x, compute A048864(x) and tally */
    for (int x = 1; x <= MAX_X; ++x) {
        int phi_x  = phi[x];
        /* π(x−1) – defined as 0 for x=1 */
        int pi_xm1 = (x > 1) ? pi[x - 1] : 0;
        int mult;  /* ω(x) if composite, otherwise 0   */
        if (x >= 2 && is_comp[x]) {
            mult = omega[x];
        } else {
            mult = 0;
        }
        int nn = phi_x - pi_xm1 + mult; /* A048864(x) */
        if (nn > 0 && nn <= MAX_N) {
            count_arr[nn] = count_arr[nn] + 1;
        }
    }

    /* free temporary storage */
    free(phi);
    free(pi);
    free(omega);
    free(is_comp);
    free(primes);

    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Public interface: returns a(n) for n ≥ 1.
   The function lazily initializes its tables on the first call. */
seq_t a(idx_t n)
{
    if (!initialized) precompute();

    if (n >= 1 && n <= (idx_t)MAX_N)
        return count_arr[n];
    else
        return 0;   /* out of range – should not happen for n ≤ 10000 */
}
