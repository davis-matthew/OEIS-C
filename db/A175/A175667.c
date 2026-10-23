/*
 * A175667  Smallest number m such that phi(m) = n*tau(m), with phi=A000010
 *          and tau=A000005; a(n)=0 if no such m exists.
 * https://oeis.org/A175667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = smallest m such that φ(m) = n·τ(m); a(n)=0 if none
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* maximum m to sieve; should be large enough so that
 * every n up to 10000 finds its m within this range
 * (empirically 2 000 000 is more than enough) */
#define SIEVE_MAX 2000000
/* maximum n we precompute answers for */
#define NMAX 200000

/* static arrays for the sieve and precomputed data */
static bool initialized = false;
static uint32_t *spf   = NULL; /* smallest prime factor */
static uint64_t *phi_arr = NULL; /* φ(m) */
static uint32_t *tau_arr = NULL; /* τ(m) */
static seq_t *ans     = NULL;   /* minimal m for each n */

/* build the sieve, φ and τ arrays and fill ans[ ] */
static void prepare(void)
{
    if (initialized) return;

    /* allocate arrays */
    spf      = (uint32_t*)calloc(SIEVE_MAX + 1, sizeof(uint32_t));
    phi_arr  = (uint64_t*)calloc(SIEVE_MAX + 1, sizeof(uint64_t));
    tau_arr  = (uint32_t*)calloc(SIEVE_MAX + 1, sizeof(uint32_t));
    ans      = (seq_t*)calloc(NMAX + 1, sizeof(seq_t));

    /* simple linear sieve for spf */
    for (uint32_t i = 2; i <= SIEVE_MAX; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            if ((uint64_t)i * i <= SIEVE_MAX) {
                for (uint64_t j = (uint64_t)i * i; j <= SIEVE_MAX; j += i)
                    if (!spf[j]) spf[j] = i;
            }
        }
    }

    /* φ(1)=1, τ(1)=1 */
    phi_arr[1] = 1;
    tau_arr[1] = 1;

    /* compute φ and τ for all m */
    for (uint32_t m = 2; m <= SIEVE_MAX; ++m) {
        uint32_t tmp = m;
        uint64_t phi = m;
        uint32_t tau = 1;
        while (tmp > 1) {
            uint32_t p = spf[tmp];
            uint32_t e  = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            phi = phi / p * (p - 1);
            tau *= (e + 1);
        }
        phi_arr[m] = phi;
        tau_arr[m] = tau;
    }

    /* fill ans[ ] with the smallest m for each n ≤ NMAX */
    for (uint32_t m = 1; m <= SIEVE_MAX; ++m) {
        uint64_t phi = phi_arr[m];
        uint32_t tau = tau_arr[m];
        if (phi % tau == 0) {
            uint64_t n = phi / tau;
            if (n <= (uint64_t)NMAX && ans[(idx_t)n] == (seq_t)0) {
                ans[(idx_t)n] = (seq_t)m;
            }
        }
    }

    initialized = true;
}

/* public function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined, but safe */
    if (n == 1) return (seq_t)1;      /* by definition */

    prepare();

    if ((idx_t)n <= (idx_t)NMAX && ans[(idx_t)n] != (seq_t)0)
        return ans[(idx_t)n];

    /* fallback search up to SIEVE_MAX if precomputation missed it */
    uint64_t ut_n = (uint64_t)n;
    seq_t result = 0;
    for (uint32_t m = 2 * ut_n + 1; m <= SIEVE_MAX; ++m) {
        uint64_t phi = phi_arr[m];
        uint32_t tau = tau_arr[m];
        seq_t rhs = (seq_t)ut_n * (seq_t)tau;
        if ((seq_t)phi == rhs) {
            result = (seq_t)m;
            break;
        }
    }

    /* store found value for future calls */
    if (result != (seq_t)0 && n <= (idx_t)NMAX)
        ans[(idx_t)n] = result;

    return result;
}
