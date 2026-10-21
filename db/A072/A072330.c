/*
 * A072330  Common difference n such that primitive triangles exist which are
 *          n-arithmetic (i.e., primitive Heronian triangles whose sides in
 *          arithmetic progression have common difference n).
 * https://oeis.org/A072330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 5299, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   allowed_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A072330: the n‑th positive integer all of whose prime factors are
   congruent to 1 or 11 modulo 12 (including 1).  This is equivalent to
   the common difference of a primitive Heronian triangle with sides in an
   arithmetic progression. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/allowed_prime.h"

/* ----------  global state  ----------------------------------------------- */
static idx_t spf_limit = 0;        /* highest number for which SPF is built   */
static idx_t *spf = NULL;          /* smallest prime factor for each number    */

static seq_t *seq = NULL;          /* memoised terms of the sequence          */
static size_t seq_len = 0;         /* number of terms currently stored        */
static size_t seq_cap = 0;         /* capacity of the seq array               */

static idx_t last_checked = 1;     /* last integer tested for allowedness     */

/* Recreate the SPF table up to at least 'limit' (inclusive).  */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_limit) return;

    /* choose a new limit that is a power of two and at least 'limit' */
    idx_t new_limit = (spf_limit == 0) ? 16384 : spf_limit;
    while (new_limit < limit) new_limit <<= 1;

    idx_t *new_spf = (idx_t *)calloc(new_limit + 1, sizeof(idx_t));
    if (!new_spf) exit(1);

    for (idx_t i = 2; i * i <= new_limit; ++i) {
        if (!new_spf[i]) {
            new_spf[i] = i;
            for (idx_t j = i * i; j <= new_limit; j += i)
                if (!new_spf[j]) new_spf[j] = i;
        }
    }
    for (idx_t i = 2; i <= new_limit; ++i)
        if (!new_spf[i]) new_spf[i] = i;

    free(spf);
    spf = new_spf;
    spf_limit = new_limit;
}

/* Test whether every prime factor of x is allowed.  */
static bool is_allowed(idx_t x)
{
    if (x <= 1) return true;
    while (x > 1) {
        idx_t p = spf[x];
        if (!allowed_prime(p)) return false;
        do { x /= p; } while (x % p == 0);
    }
    return true;
}

/* Reallocate the term array if needed.  */
static void ensure_seq_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!seq) exit(1);
    seq_cap = new_cap;
}

/* ----------  public function  ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 0 */

    /* lazy initialization of sieve and first term */
    if (!seq) {
        ensure_spf(16384);
        seq = (seq_t *)malloc(16 * sizeof(seq_t));
        if (!seq) exit(1);
        seq[0] = (seq_t)1;      /* a(1) = 1 */
        seq_len = 1;
        seq_cap = 16;
    }

    /* generate terms until we have n of them */
    while (seq_len < n) {
        ++last_checked;
        ensure_spf(last_checked);
        if (is_allowed(last_checked)) {
            ensure_seq_cap(seq_len + 1);
            seq[seq_len++] = (seq_t)last_checked;
        }
    }

    return seq[n - 1];
}
