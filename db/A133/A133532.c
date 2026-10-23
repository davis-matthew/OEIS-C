/*
 * A133532  Sum of fifth powers of three consecutive primes.
 * https://oeis.org/A133532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 18, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A133532:  Sum of fifth powers of three consecutive primes.
   Formula: a(n) = p_n^5 + p_{n+1}^5 + p_{n+2}^5   (n ≥ 1)
   where p_k is the k‑th prime (p_1 = 2). */

#include <stdlib.h>
#include <stdbool.h>

static uint64_t *prime_list = NULL;    /* cache of primes 2,3,5,... */
static size_t   prime_count = 0;      /* number of primes cached */

/* ---- prime generation ----------------------------------------------------- */
static idx_t estimate_limit(idx_t need)
{
    /* Sufficient upper bound for the first `need` primes.
       For n ≥ 6, p_n < n(ln n + ln ln n).  A small multiple is enough. */
    if (need < 6)
        return 15;                     /* covers up to the 5th prime */
    return need * 20 + 100;            /* generous but trivial */
}

static void ensure_primes(idx_t need)
{
    if (prime_count >= (size_t)need)
        return;                       /* already have enough */

    idx_t limit = estimate_limit(need);
    char *comp = (char *)calloc((size_t)limit + 1, sizeof(char));
    if (!comp)
        return;                       /* allocation failed – give up */

    for (idx_t i = 2; i * i <= (uint64_t)limit; ++i)
        if (!comp[i])
            for (uint64_t j = i * i; j <= (uint64_t)limit; j += i)
                comp[j] = 1;

    /* Count primes and allocate array */
    size_t cnt = 0;
    for (idx_t i = 2; i <= (uint64_t)limit; ++i)
        if (!comp[i])
            ++cnt;

    uint64_t *tmp = (uint64_t *)calloc(cnt, sizeof(uint64_t));
    if (tmp) {
        size_t idx = 0;
        for (idx_t i = 2; i <= (uint64_t)limit; ++i)
            if (!comp[i])
                tmp[idx++] = (uint64_t)i;
    }

    free(comp);

    free(prime_list);
    prime_list = tmp;
    prime_count = cnt;
}

/* ---- main sequence function ---------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;        /* undefined for n < 1 */

    idx_t need = n + 2;            /* indices n-1, n, n+1 (0‑based) */
    ensure_primes(need);
    if (prime_count < (size_t)need)
        return (seq_t)0;          /* error: not enough primes cached */

    seq_t p1 = (seq_t)prime_list[n - 1];
    seq_t p2 = (seq_t)prime_list[n];
    seq_t p3 = (seq_t)prime_list[n + 1];

    /* compute p^5 via successive multiplication */
    seq_t sum = 0, mul;

    mul = p1;  mul *= p1;  mul *= p1;  mul *= p1;  mul *= p1;
    sum += mul;

    mul = p2;  mul *= p2;  mul *= p2;  mul *= p2;  mul *= p2;
    sum += mul;

    mul = p3;  mul *= p3;  mul *= p3;  mul *= p3;  mul *= p3;
    sum += mul;

    return sum;
}
