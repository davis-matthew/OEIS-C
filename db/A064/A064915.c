/*
 * A064915  Positive integers n that are not of the form p + q * a^2, where p
 *          and q are primes and a is the smallest prime not dividing n.
 * https://oeis.org/A064915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A064915
   a(n) = the nth positive integer that is NOT of the form
          p + q * a^2, where p and q are primes and a is the
          smallest prime not dividing n.  The function tests
          integers sequentially, accumulating the acceptable
          ones until the nth is found. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ---------- Prime storage and sieve ---------- */

static idx_t      curr_limit   = 0;          /* Highest number for which we know primality */
static unsigned char *isPrime   = NULL;      /* 0/1 array of primality */
static size_t      isPrime_cap  = 0;         /* Allocated size of isPrime */

static uint32_t   *primes      = NULL;      /* Array of primes (>=2) */
static size_t      primes_cap  = 0;         /* Allocated capacity for primes */
static size_t      primes_cnt  = 0;         /* Number of primes stored */

/* Ensure that primes up to 'limit' are available. */
static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= curr_limit) return;

    /* Expand the isPrime array if necessary */
    if ((size_t)limit + 1 > isPrime_cap) {
        unsigned char *newp = (unsigned char*)realloc(isPrime, (size_t)limit + 1);
        if (!newp) abort();
        memset(newp + isPrime_cap, 0, (size_t)limit + 1 - isPrime_cap);
        isPrime     = newp;
        isPrime_cap = limit + 1;
    }

    /* Extend primes list and sieve for new numbers */
    for (idx_t i = curr_limit + 1; i <= limit; ++i) {
        if (i < 2) {                   /* skip 0 and 1 */
            isPrime[i] = 0;
            continue;
        }
        bool prime = true;
        for (size_t pi = 0; pi < primes_cnt; ++pi) {
            uint32_t p = primes[pi];
            if ((idx_t)p * (idx_t)p > i) break;
            if (i % p == 0) { prime = false; break; }
        }
        if (prime) {
            if (primes_cnt == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 64;
                uint32_t *newp = (uint32_t*)realloc(primes, new_cap * sizeof(uint32_t));
                if (!newp) abort();
                primes     = newp;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = (uint32_t)i;
            isPrime[i] = 1;
        } else {
            isPrime[i] = 0;
        }
    }

    isPrime[0] = isPrime[1] = 0;  /* 0 and 1 are not prime */
    curr_limit = limit;
}

/* Check primality using the pre‑computed table.  The table is expanded if
 * necessary. */
static inline bool is_prime_idx(idx_t x)
{
    if (x > curr_limit) ensure_primes_up_to(x);
    return isPrime[x];
}

/* ---------- Sequence storage ---------- */

static seq_t *seq_arr  = NULL;   /* Array of sequence terms            */
static size_t seq_len  = 0;      /* Number of terms stored             */
static size_t seq_cap  = 0;      /* Allocated capacity of seq_arr      */
static idx_t  cur_n    = 1;      /* Current integer being examined     */

/* Ensure the sequence array has capacity at least 'need'. */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 32;
    while (new_cap < need) new_cap <<= 1;
    seq_arr = (seq_t*)realloc(seq_arr, new_cap * sizeof(seq_t));
    if (!seq_arr) abort();
    seq_cap = new_cap;
}

/* The main function: returns the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined for n < 1 but harmless */

    /* Grow the sequence until we have at least n terms. */
    while (seq_len < (size_t)n) {
        idx_t cand = cur_n;   /* Candidate integer to test */

        /* We need primes up to *at least* cand+1 to find the
           smallest prime not dividing cand. */
        ensure_primes_up_to(cand + 1);

        /* Find the smallest prime not dividing 'cand'. */
        idx_t a_small = 0;
        for (size_t pi = 0; pi < primes_cnt; ++pi) {
            idx_t p = primes[pi];
            if (cand % p != 0) { a_small = p; break; }
        }
        if (a_small == 0) a_small = 2;   /* safety for cand == 1 */

        idx_t a2 = a_small * a_small;
        bool excluded = false;

        /* Quick elimination: if even the smallest possible p (2) makes
           p + q*a2 > cand, we skip the inner loop. */
        if (cand >= 2 + a2) {
            idx_t max_q = (cand - 2) / a2;
            for (size_t qi = 0; qi < primes_cnt && primes[qi] <= max_q; ++qi) {
                idx_t q = primes[qi];
                idx_t p_val = cand - q * a2;
                if (p_val < 2) continue;
                if (!is_prime_idx(p_val)) continue;
                excluded = true;
                break;
            }
        }

        if (!excluded) {
            /* Include this number in the sequence. */
            ensure_seq_cap(seq_len + 1);
            seq_arr[seq_len++] = (seq_t)cand;
        }

        ++cur_n;  /* Move to next integer */
    }

    return seq_arr[(size_t)n - 1];
}
