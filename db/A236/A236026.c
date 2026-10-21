/*
 * A236026  Sequence of numbers from A236025 such that A236025(n) > A236025(k)
 *          for all k < n.
 * https://oeis.org/A236026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth record‑breaking composite from A236025
 *            (composites sorted by increasing delta(n) = sqrt(n+1) –
 *            sigma(n)^(1/tau(n)))
 * Range:     u8 n <= 30, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#ifdef __cplusplus
#include <new>
#endif

/* --- static data ---------------------------------------------------- */

#define LIMIT 5000000U            /* upper bound for precomputation   */
#define INITIAL_CAP 16384U

/* auxiliary arrays for the sieve */
static uint32_t *spf = NULL;        /* smallest prime factor           */
static uint64_t *sigma_arr = NULL;  /* sigma(n)                        */
static uint16_t *tau_arr  = NULL;   /* tau(n) (number of divisors)     */

/* composite numbers with their delta value */
typedef struct { uint32_t n; double delta; } comp_t;
static comp_t *comps = NULL;
static size_t comp_cnt = 0;

/* record‑breaking values of A236026 */
static seq_t *records = NULL;
static size_t rec_len = 0;
static size_t rec_cap = 0;

static bool initialized = false;

/* --- helper functions ---------------------------------------------- */

/* comparison for qsort: by delta ascending, then by n ascending */
static int comp_cmp(const void *a, const void *b)
{
    const comp_t *A = (const comp_t *)a;
    const comp_t *B = (const comp_t *)b;
    if (A->delta < B->delta) return -1;
    if (A->delta > B->delta) return 1;
    if (A->n < B->n) return -1;
    if (A->n > B->n) return 1;
    return 0;
}

/* ensure the records array has enough capacity to store an additional
   element.  C++ objects are constructed using placement new when
   compiled as C++. */
static void ensure_record_capacity(void)
{
    if (rec_cap == 0) {
        rec_cap = INITIAL_CAP;
        records = (seq_t *)malloc(rec_cap * sizeof(seq_t));
#ifdef __cplusplus
        for (size_t i = 0; i < rec_cap; ++i) new (&records[i]) seq_t();
#endif
    } else if (rec_len >= rec_cap) {
        rec_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(records, rec_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);        /* unrecoverable in this context */
        records = tmp;
#ifdef __cplusplus
        for (size_t i = rec_len; i < rec_cap; ++i) new (&records[i]) seq_t();
#endif
    }
}

/* --- initialization ------------------------------------------------- */
static void init(void)
{
    if (initialized) return;

    /* 1. allocate basic arrays */
    spf        = (uint32_t *)calloc(LIMIT + 1, sizeof(uint32_t));
    sigma_arr  = (uint64_t *)calloc(LIMIT + 1, sizeof(uint64_t));
    tau_arr    = (uint16_t *)calloc(LIMIT + 1, sizeof(uint16_t));

    /* 2. linear sieve for smallest prime factor */
    uint32_t *primes = (uint32_t *)malloc((LIMIT / 10) * sizeof(uint32_t));
    size_t primes_cnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) spf[i] = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            if (p > spf[i] || (uint64_t)i * p > LIMIT) break;
            spf[i * p] = p;
        }
    }
    free(primes);

    /* 3. compute sigma(n) and tau(n) by factorisation using spf */
    sigma_arr[1] = 1;
    tau_arr[1]   = 1;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        uint32_t n = i;
        uint64_t sigma_val = 1;
        uint16_t tau_val   = 1;
        while (n > 1) {
            uint32_t p = spf[n];
            uint32_t exp = 0;
            uint64_t pow_p = 1;
            uint64_t term = 1;
            while (n % p == 0) {
                n /= p;
                exp++;
                pow_p *= p;
                term += pow_p;
            }
            sigma_val *= term;
            tau_val   *= (exp + 1);
        }
        sigma_arr[i] = sigma_val;
        tau_arr[i]   = tau_val;
    }

    /* 4. collect all composite numbers with their delta value */
    comps = (comp_t *)malloc((LIMIT - 3) * sizeof(comp_t));  /* 4..LIMIT */
    comp_cnt = 0;
    for (uint32_t n = 4; n <= LIMIT; ++n) {
        if (tau_arr[n] <= 2) continue;          /* skip primes and 1 */
        double delta = sqrt((double)(n + 1))
                     - pow((double)sigma_arr[n], 1.0 / (double)tau_arr[n]);
        comps[comp_cnt].n = n;
        comps[comp_cnt].delta = delta;
        ++comp_cnt;
    }

    /* 5. sort composites by increasing delta (and by n for ties) */
    qsort(comps, comp_cnt, sizeof(comp_t), comp_cmp);

    /* 6. extract record‑breaking values */
    for (size_t i = 0; i < comp_cnt; ++i) {
        uint32_t n = comps[i].n;
        if (rec_len == 0 || n > (uint32_t)records[rec_len - 1]) {
            ensure_record_capacity();
            records[rec_len++] = (seq_t)n;
        }
    }

    /* 7. free temporary arrays that are no longer needed */
    free(comps);
    free(spf);
    free(sigma_arr);
    free(tau_arr);

    initialized = true;
}

/* --- public API ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* out of bounds under the OEIS offset */
    init();
    if ((idx_t)rec_len >= n) return records[n - 1];
    return 0;                  /* n exceeds the pre‑computed range  */
}
