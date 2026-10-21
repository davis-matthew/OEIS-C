/*
 * A063078  Numbers k such that phi(k)*mu(k) > phi(m)*mu(m) for all m < k.
 * https://oeis.org/A063078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A063078: numbers k such that φ(k)*μ(k) is a record
   (greater than all previous φ(m)*μ(m)).  The sequence consists
   solely of the positive record values of φ(k) with μ(k)=+1.  */
/* The function builds the list of records incrementally and returns
   the n‑th element (n ≥ 1). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
#include <vector>
#endif

/* ------------------------------------------------------------------ */
/*  Facilities for building the list of record holders                 */
/* ------------------------------------------------------------------ */
#ifdef __cplusplus
static std::vector<seq_t> records;
static idx_t processed_limit = 0;
static inline void clear_records(void) { records.clear(); }
static inline void push_record(seq_t v) { records.push_back(v); }
static inline size_t record_count(void) { return records.size(); }
static inline seq_t record_at(size_t i) { return records[i]; }
#else /* C */
static seq_t *records = NULL;
static size_t record_cap   = 0;
static size_t record_len   = 0;
static idx_t processed_limit = 0;
static inline void clear_records(void)
{
    free(records);
    records = NULL;
    record_cap = 0;
    record_len = 0;
}
static inline void push_record(seq_t v)
{
    if (record_len == record_cap)
    {
        record_cap = record_cap ? record_cap << 1 : 16;
        records = (seq_t *)realloc(records, record_cap * sizeof(seq_t));
    }
    records[record_len++] = v;
}
static inline size_t record_count(void) { return record_len; }
static inline seq_t record_at(size_t i) { return records[i]; }
#endif

/* ------------------------------------------------------------------ */
/*  Linear sieve for φ and μ up to a limit                              */
/* ------------------------------------------------------------------ */
static void recompute(idx_t limit)
{
    /* Temporary arrays for φ (idx_t) and μ (int8_t).  They are plain
       data types, so plain malloc/realloc/free works in both C and C++. */
    idx_t *phi = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    int8_t *mu  = (int8_t *)malloc((limit + 1) * sizeof(int8_t));
    bool *composite = (bool *)malloc((limit + 1) * sizeof(bool));

    size_t primes_cap = (size_t)(limit / 10) + 10;         /* good estimate */
    uint32_t *primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    size_t primes_cnt = 0;

    /* Initialise */
    for (idx_t i = 0; i <= limit; ++i) composite[i] = false;
    phi[1] = 1;
    mu[1]  = 1;

    for (idx_t i = 2; i <= limit; ++i)
    {
        if (!composite[i])                /* i is prime */
        {
            if (primes_cnt == primes_cap)
            {
                primes_cap <<= 1;
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_cnt++] = (uint32_t)i;
            phi[i] = i - 1;
            mu[i]  = -1;
        }
        for (size_t j = 0; j < primes_cnt; ++j)
        {
            idx_t p   = primes[j];
            idx_t ip  = i * p;
            if (ip > limit) break;
            composite[ip] = true;
            if (i % p == 0)          /* p divides i */
            {
                phi[ip] = phi[i] * p;
                mu[ip]  = 0;
                break;
            }
            else
            {
                phi[ip] = phi[i] * (p - 1);
                mu[ip]  = -mu[i];
            }
        }
    }

    /* ------------------------------------------------------------------
       Extract the record holders: μ(k)=+1 and φ(k) larger than any
       previous record.  We store only the k values.
       ------------------------------------------------------------------ */
    clear_records();                      /* start fresh for this limit */
    push_record((seq_t)1);                 /* k = 1 is always a record */
    seq_t max_phi = (seq_t)phi[1];         /* φ(1)=1 */

    for (idx_t k = 2; k <= limit; ++k)
    {
        if (mu[k] != 1) continue;         /* need μ(k)=+1 */
        seq_t f = (seq_t)phi[k];
        if (f > max_phi)
        {
            push_record((seq_t)k);
            max_phi = f;
        }
    }

    /* Clean up */
    free(phi);
    free(mu);
    free(composite);
    free(primes);

    processed_limit = limit;
}

/* ------------------------------------------------------------------ */
/*  Ensure that at least N record values have been computed             */
/* ------------------------------------------------------------------ */
static void ensure_records(idx_t N)
{
    if (record_count() >= N) return;     /* nothing to do */

    /* Start with some limit and double it until we have enough records. */
    idx_t lim = processed_limit ? processed_limit : 2;
    while (record_count() < N)
    {
        recompute(lim);
        if (record_count() >= N) break;
        /* Double the limit for next iteration, guarding against overflow. */
        if (lim > ((idx_t)(-1)) >> 1) lim = (idx_t)(-1);   /* cap at max */
        else lim <<= 1;
    }
}

/* ------------------------------------------------------------------ */
/*  Public function required by the OEIS build system                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined; return 0 for safety */
    ensure_records(n);
    return record_at((size_t)(n - 1));
}
