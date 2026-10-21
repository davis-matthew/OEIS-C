/*
 * A235623  Numbers n for which in the prime power factorization of n!, the
 *          numbers of exponents 1 and >1 are equal.
 * https://oeis.org/A235623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A235623 – the sequence of numbers n for which
   in the prime‑power factorisation of n! the amount of exponents 1
   equals the amount of exponents >1.  By the OEIS remark this
   is equivalent to the condition pi(n) = 2·pi(n/2).
   The implementation sieves primes progressively and records
   all terms satisfying that equality.  It is valid for any
   seq_t size and runs in O(N log log N), suitable for
   n up to about 10 000. */

#include <stdint.h>
#include <stdlib.h>

static seq_t *seq_vals = NULL;   /* array of found terms */
static size_t seq_len = 0, seq_cap = 0;

static uint8_t *is_prime = NULL;
static uint32_t *pi_arr   = NULL;
static idx_t sieve_limit = 0;  /* highest value sieved so far   */
static idx_t last_scanned = 1; /* last number processed in the sequence */

static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;

    if (is_prime) free(is_prime);
    if (pi_arr)   free(pi_arr);

    is_prime = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
    if (!is_prime) abort();
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    pi_arr = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    if (!pi_arr) abort();
    uint32_t cnt = 0;
    for (idx_t i = 0; i <= limit; ++i) {
        if (is_prime[i]) ++cnt;
        pi_arr[i] = cnt;
    }

    sieve_limit = limit;
}

static void append_seq(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *tmp = (seq_t*)realloc(seq_vals, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_vals = tmp;
        seq_cap  = new_cap;
    }
    seq_vals[seq_len++] = val;
}

static void extend_up_to(idx_t n_req)
{
    while (seq_len < n_req) {
        idx_t new_limit = (sieve_limit == 0) ? 1024 : sieve_limit * 2;
        if (new_limit <= last_scanned) new_limit = last_scanned + 1;
        ensure_sieve(new_limit);

        for (idx_t m = last_scanned + 1; m <= new_limit; ++m)
            if ((uint32_t)pi_arr[m] == 2U * pi_arr[m/2])
                append_seq((seq_t)m);

        last_scanned = new_limit;
    }
}

seq_t a(idx_t n)   /* n ≥ 1 */
{
    static int init = 0;
    if (!init) {
        append_seq((seq_t)0);   /* a(1) = 0 */
        append_seq((seq_t)1);   /* a(2) = 1 */
        init = 1;
    }
    extend_up_to(n);
    return seq_vals[n - 1];
}
