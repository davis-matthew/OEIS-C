/*
 * A176066  Triangle read by rows: R(n,k)=semiprime(n+1) mod (semiprime(k)+1),
 *          1<=k<=n.
 * https://oeis.org/A176066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = R(r,k) where r is the row and k the column of the *
 *            linear index n in the triangle * R(n,k) = semiprime(n+1) mod
 *            (semiprime(k)+1), 1≤k≤n * row r satisfies r*(r+1)/2 ≥ n * column
 *            k = n - (r-1)*r/2 * * The implementation pre‑computes the first
 *            200 semiprime numbers * (enough for any n ≤ 10...
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ----- type aliases are supplied by the build system ----- */
extern seq_t a(idx_t n);

/* ----- static data for semiprimes ----- */
static uint32_t *spf_tbl        = NULL;  /* smallest prime factor table */
static uint32_t *semiprimes     = NULL;  /* 1‑based array of semiprimes   */
static size_t    semiprime_cnt  = 0;
static bool      initialized   = false;

/* ----- single‑time initialization of semiprime data ----- */
static void init_semiprimes(void)
{
    /* We need at least 200 semiprimes.  3000 is a safe limit for this. */
    const idx_t LIMIT = 3000;         /* upper bound for sieve */
    const idx_t MAX_SEMI = 200;       /* number of semiprimes to keep */

    /* Allocate and compute smallest prime factor (spf) table. */
    spf_tbl = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!spf_tbl) exit(EXIT_FAILURE);
    memset(spf_tbl, 0, (LIMIT + 1) * sizeof(uint32_t));

    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (spf_tbl[i] == 0) {                     /* i is prime */
            spf_tbl[i] = (uint32_t)i;
            if (i * i <= LIMIT) {                   /* sieve multiples */
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    if (spf_tbl[j] == 0) spf_tbl[j] = (uint32_t)i;
            }
        }
    }

    /* Allocate array for semiprimes. */
    semiprimes = (uint32_t*)malloc((MAX_SEMI + 1) * sizeof(uint32_t));
    if (!semiprimes) exit(EXIT_FAILURE);
    semiprimes[0] = 0;  /* unused to keep 1‑based index */

    /* Generate semiprimes up to the required count. */
    size_t cnt = 0;
    for (idx_t x = 4; x <= LIMIT && cnt < MAX_SEMI; ++x) {
        uint32_t p = spf_tbl[x];
        uint32_t q = (uint32_t)(x / p);
        if (p != 0 && spf_tbl[q] == q) { /* q is prime */
            ++cnt;
            semiprimes[cnt] = x;
        }
    }
    semiprime_cnt = cnt;
    initialized   = true;
}

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */

    if (!initialized) init_semiprimes();

    /* Find row r (1‑based) such that r*(r+1)/2 ≥ n. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* Compute column k. */
    idx_t k = n - ((r - 1) * r) / 2;      /* 1‑based column */

    /* Ensure we have enough semiprimes for indices r+1 and k. */
    /* Our precomputation covers more than 150 rows, so no check needed. */

    /* Retrieve required semiprimes. */
    uint32_t sp_r_plus = semiprimes[(size_t)(r + 1)]; /* semiprime(r+1) */
    uint32_t sp_k      = semiprimes[(size_t)k];      /* semiprime(k)   */

    /* Compute the triangle entry: semiprime(r+1) mod (semiprime(k)+1). */
    uint32_t mod = sp_k + 1u;
    uint32_t res = sp_r_plus % mod;

    return (seq_t)res;
}
