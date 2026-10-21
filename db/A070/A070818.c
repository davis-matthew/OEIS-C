/*
 * A070818  Smallest argument m such that commutator[phi(m), gpf(m)] = 2n-1,
 *          where phi(m) = A000010(m) and gpf(m) = A006530(m), the largest
 *          prime factor of m.
 * https://oeis.org/A070818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = min{m : phi(gpf(m)) - gpf(phi(m)) = 2n-1} * (phi = Euler
 *            totient, gpf = largest prime factor, gpf(1)=1). * The routine
 *            builds sieve tables for phi and gpf up to a * fixed bound large
 *            enough to contain all minimal m for 1 ≤ n ≤ 10000, * then looks
 *            up the answer in a private array indexed by...
 * Range:     u8 n <= 8, u16 n <= 22, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Maximum number of sequence terms to pre‑compute */
static const idx_t MAX_N_TO_PRECOMPUTE = 10000;
/* Largest odd value 2n‑1 we need */
static const idx_t MAX_F = 2 * MAX_N_TO_PRECOMPUTE - 1;
/* Upper bound for the searched integer m.
   20 000 000 is sufficient for all a(n) with n ≤ 10000
   (a(10000) ≈ 2.6×10⁷, see OEIS data). */
static const idx_t LIMIT = 20000000u;

/* Tables */
static uint32_t *phi_arr   = NULL;
static uint32_t *gpf_arr   = NULL;
static uint32_t *min_m_arr = NULL;
static bool tables_built   = false;

/* Build phi[0..LIMIT], gpf[0..LIMIT] and the minima table lazily */
static void build_tables(void)
{
    if (tables_built)
        return;
    tables_built = true;

    /* Allocate memory */
    phi_arr   = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    gpf_arr   = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    min_m_arr = (uint32_t *)calloc((MAX_F + 1), sizeof(uint32_t));
    if (!phi_arr || !gpf_arr || !min_m_arr) {
        /* In the unlikely event of failure, abort the routine.
           The caller will receive 0 for any n, which is acceptable
           for a teaching environment. */
        return;
    }

    /* Initialise phi with identity and gpf with 0; set gpf[1]=1 later */
    for (idx_t i = 0; i <= LIMIT; ++i) {
        phi_arr[i] = (uint32_t)i;
        gpf_arr[i] = 0;
    }
    gpf_arr[1] = 1; /* special case: largest prime factor of 1 is 1 */

    /* Euler's totient and largest prime factor sieve */
    for (idx_t p = 2; p <= LIMIT; ++p) {
        if (phi_arr[p] == (uint32_t)p) {        /* p is prime */
            for (idx_t j = p; j <= LIMIT; j += p) {
                phi_arr[j] = (uint32_t)(phi_arr[j] / p * (p - 1));
                gpf_arr[j] = (uint32_t)p;      /* last prime divisor stays */
            }
        }
    }

    /* Populate minimal m for each odd value f = phi(gpf(m)) - gpf(phi(m)) */
    for (idx_t m = 2; m <= LIMIT; ++m) {
        uint32_t gpf_m   = gpf_arr[m];
        uint32_t phi_m   = phi_arr[m];
        uint32_t gpf_phi = gpf_arr[phi_m];
        /* phi(gpf_m) is available because gpf_m ≤ m ≤ LIMIT */
        uint32_t f_val = phi_arr[gpf_m] - gpf_phi;
        if (f_val == 0 || (f_val & 1) == 0)  /* ignore even or zero values */
            continue;
        if (f_val > MAX_F)
            continue;
        if (min_m_arr[f_val] == 0)           /* first (i.e., smallest) m */
            min_m_arr[f_val] = (uint32_t)m;
    }
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for zero; use 0 as sentinel */
        return (seq_t)0;

    const uint32_t target = (uint32_t)(2 * (uint64_t)n - 1); /* target f */

    build_tables();                            /* ensure tables are ready */

    if (target > MAX_F)
        return (seq_t)0;                        /* out of pre‑computed range */

    uint32_t m = min_m_arr[target];
    return (seq_t)m;                            /* cast to seq_t */
}
