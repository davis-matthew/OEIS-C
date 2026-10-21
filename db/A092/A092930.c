/*
 * A092930  n-th partial arithmetic mean is equal to the n-th composite
 *          number.
 * https://oeis.org/A092930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * c_n - S(n-1), where c_n is the n‑th composite number
 *            and S(k) = a(1)+…+a(k) (with S(0)=0). This recurrence follows
 *            from (a(1)+…+a(n))/n = c_n
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Global cache of the first few composite numbers. */
static seq_t *comps = NULL;          /* composites, 0‑based index */
static idx_t   comps_cnt  = 0;       /* number actually stored */
static size_t  comps_cap  = 0;       /* allocated capacity */

/* Global cache of the sequence values and their cumulative sums. */
static seq_t *a_arr   = NULL;        /* a(n), 0‑based index */
static seq_t *sum_arr = NULL;        /* sum_arr[i] = a(1)+…+a(i), sum_arr[0]=0 */
static idx_t   a_cnt   = 0;           /* number of a(n) values stored */
static size_t  a_cap   = 0;           /* allocated capacity */

/* ------------------------------------------------------------ */
/* Sieve to generate at least `need` composite numbers.         */
/* ------------------------------------------------------------ */
static void ensure_comps(idx_t need)
{
    if (comps_cnt >= need) return;

    /* Estimate an upper bound for the nth composite. */
    idx_t limit = need * 20 + 100;
    if (limit < 100) limit = 100;

    /* Increase limit until at least `need` composites are found. */
    while (1) {
        uint8_t *is_prime = (uint8_t*)malloc((size_t)(limit + 1));
        if (!is_prime) return;      /* allocation failure → give up */

        for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;

        for (idx_t p = 2; p * p <= limit; ++p)
            if (is_prime[p])
                for (idx_t m = p * p; m <= limit; m += p)
                    is_prime[m] = 0;

        idx_t count = 0;
        for (idx_t i = 4; i <= limit && count < need; ++i)
            if (!is_prime[i]) ++count;

        free(is_prime);

        if (count >= need) break;
        limit *= 2;                  /* not enough → enlarge */
    }

    /* Allocate or enlarge storage for composites. */
    if (comps_cap < need) {
        comps_cap = need * 2;
        seq_t *tmp = (seq_t*)realloc(comps, comps_cap * sizeof(seq_t));
        comps = tmp;
    }

    /* Re‑compute the composites list up to `need`. */
    uint8_t *is_prime = (uint8_t*)malloc((size_t)(limit + 1));
    if (!is_prime) return;
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (idx_t p = 2; p * p <= limit; ++p)
        if (is_prime[p])
            for (idx_t m = p * p; m <= limit; m += p)
                is_prime[m] = 0;

    comps_cnt = 0;
    for (idx_t i = 4; i <= limit && comps_cnt < need; ++i)
        if (!is_prime[i])
            comps[comps_cnt++] = (seq_t)i;

    free(is_prime);
}

/* ------------------------------------------------------------ */
/* Ensure that a(1)…a(n) are computed and stored.               */
/* ------------------------------------------------------------ */
static void ensure_a(idx_t n)
{
    if (a_cnt >= n) return;

    ensure_comps(n);

    if (a_cap < n) {
        a_cap = n * 2;
        seq_t *tmp_a   = (seq_t*)realloc(a_arr,   a_cap * sizeof(seq_t));
        seq_t *tmp_sum = (seq_t*)realloc(sum_arr, (a_cap + 1) * sizeof(seq_t));
        a_arr   = tmp_a;
        sum_arr = tmp_sum;
    }

    /* sum_arr[0] must be zero. */
    sum_arr[0] = (seq_t)0;

    for (idx_t idx = a_cnt + 1; idx <= n; ++idx) {
        seq_t comp    = comps[idx - 1];          /* c_n */
        seq_t n_seq   = (seq_t)idx;             /* n as seq_t */
        seq_t sum_prev = sum_arr[idx - 1];      /* S(n-1) */
        seq_t value    = n_seq * comp - sum_prev;/* a(n) */
        a_arr[idx - 1] = value;
        sum_arr[idx]   = sum_prev + value;
    }

    a_cnt = n;
}

/* ------------------------------------------------------------ */
/* Public sequence routine.                                      */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, return zero */
    ensure_a(n);
    return a_arr[n - 1];
}
