/*
 * A161570  Sum of all numbers from n up to A018252(n).
 * https://oeis.org/A161570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000217(A018252(n)) – A000217(n-1) where A018252(n) is
 *            the (n−1)-st composite number (1‑indexed)
 * Range:     u8 n <= 19, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* state for the dynamic composite list */
static idx_t   sieve_limit     = 0;        /* current sieve upper bound      */
static size_t  composite_count = 0;        /* number of composites found     */
static idx_t  *composite_list   = NULL;    /* list of composite numbers      */

/* Build a sieve up to ‘limit’ and store all composites in composite_list.
   The previous list (if any) is freed.  This is called only when more
   composites are needed. */
static void build_sieve(idx_t limit)
{
    bool *is_prime = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) abort();

    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* Count composites to allocate the list */
    size_t cnt = 0;
    for (idx_t m = 2; m <= limit; ++m)
        if (!is_prime[m]) ++cnt;

    if (composite_list) free(composite_list);
    composite_list = (idx_t*)malloc(cnt * sizeof(idx_t));
    if (!composite_list) abort();

    size_t pos = 0;
    for (idx_t m = 2; m <= limit; ++m)
        if (!is_prime[m]) composite_list[pos++] = m;

    composite_count = cnt;
    sieve_limit = limit;
    free(is_prime);
}

/* Ensure that at least ‘needed’ composites (0‑based index) are available.  */
static void ensure_composites(idx_t needed)
{
    if (composite_count > needed) return;      /* already enough */

    idx_t new_limit = sieve_limit ? sieve_limit : 2;
    while (1) {
        new_limit *= 2;
        build_sieve(new_limit);
        if (composite_count > needed) break;
    }
}

/* ----------  public function required by the build  ---------- */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;               /* a(1) = 1 by definition */

    /* zero‑based index of the needed composite: (n‑1)-th composite (1‑based) */
    idx_t target = n - 2;

    ensure_composites(target);                  /* make sure we have it */

    idx_t m = composite_list[target];           /* the composite to sum to */

    /* a(n) = T(m) – T(n‑1) where T(x) = x(x+1)/2 */
    seq_t sm = (seq_t)m * ((seq_t)m + 1) / 2;   /* T(m) */
    seq_t sn = (seq_t)(n - 1) * (seq_t)n / 2;   /* T(n‑1) */
    return sm - sn;
}
