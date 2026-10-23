/*
 * A177266  Triangle read by rows: R(n,k) = 2^(semiprime(n)-semiprime(k)) mod
 *          semiprime(n).
 * https://oeis.org/A177266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = R(r,k) where R(r,k) = 2^(semiprime(r)-semiprime(k)) mod
 *            semiprime(r) and r,k are the row and column indices of the
 *            triangular enumeration with offset 1
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*--- Local cache of semiprimes ---------------------------------------*/
static seq_t *semiprimes = NULL;  /* array of computed semiprimes   */
static size_t semiprime_cnt = 0;  /* number of stored semiprimes    */
static size_t semiprime_cap = 0;  /* capacity of semiprimes array   */
static uint64_t last_checked = 3; /* last integer examined for semiprime */

/*--- Compute 2^exp mod modv -------------------------------------------*/
static seq_t mod_pow(seq_t base, uint64_t exp, seq_t modv)
{
    seq_t result = 1;
    seq_t b = base % modv;
    while (exp > 0) {
        if (exp & 1ULL) result = (b * result) % modv;
        b = (b * b) % modv;
        exp >>= 1ULL;
    }
    return result;
}

/*--- Simple test for semiprime: exactly two prime factors (counted with multiplicity) */
static bool is_semiprime(uint64_t x)
{
    int cnt = 0;
    uint64_t t = x;
    while ((t % 2ULL) == 0ULL) {
        t /= 2ULL;
        ++cnt;
        if (cnt > 2) return false;
    }
    for (uint64_t p = 3ULL; p * p <= t; p += 2ULL) {
        while ((t % p) == 0ULL) {
            t /= p;
            ++cnt;
            if (cnt > 2) return false;
        }
    }
    if (t > 1ULL) ++cnt;
    return cnt == 2;
}

/*--- Ensure at least 'need' semiprimes are available ------------------*/
static void ensure_semiprimes(size_t need)
{
    if (need <= semiprime_cnt) return;          /* already enough */

    /* Allocate or grow the storage as needed */
    if (semiprime_cap == 0) {
        semiprime_cap = 128;
        semiprimes = (seq_t *)malloc(semiprime_cap * sizeof(seq_t));
    }
    while (semiprime_cnt < need) {
        ++last_checked;
        if (is_semiprime(last_checked)) {
            if (semiprime_cnt == semiprime_cap) {
                semiprime_cap *= 2;
                semiprimes = (seq_t *)realloc(semiprimes,
                                              semiprime_cap * sizeof(seq_t));
            }
            semiprimes[semiprime_cnt++] = (seq_t)last_checked;
        }
    }
}

/*--- Main sequence function -------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    /* Find the row r such that r(r+1)/2 >= n */
    idx_t r = 1;
    while (((idx_t)r * ((idx_t)r + 1ULL)) / 2ULL < n) ++r;

    /* Compute column index k = n - sum_{i=1}^{r-1} i */
    idx_t prev_sum = ((idx_t)(r - 1ULL) * (idx_t)r) / 2ULL;
    idx_t k = n - prev_sum;                     /* 1‑based column */

    /* Make sure we have enough semiprimes for indices r and k */
    size_t need = (r > k) ? (size_t)r : (size_t)k;
    ensure_semiprimes(need);

    seq_t semir = semiprimes[(size_t)(r - 1ULL)];
    seq_t semik = semiprimes[(size_t)(k - 1ULL)];
    seq_t diff  = semir - semik;                 /* non‑negative */

    /* Compute 2^diff mod semir */
    seq_t result = mod_pow((seq_t)2, (uint64_t)diff, semir);
    return result;
}
