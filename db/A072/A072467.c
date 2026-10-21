/*
 * A072467  Triangle in which n-th row contains n primes == 1 mod (n) and the
 *          first term of a row is just greater than the last term of the
 *          previous row.
 * https://oeis.org/A072467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A072467 – “Triangle in which n‑th row contains n primes == 1 mod n
    and the first term of a row is the smallest prime greater than the
    last term of the previous row”.  
    The sequence is written row‑by‑row, so a(1)=2, a(2)=3, a(3)=5, ...  */

/*  The implementation builds primes on demand and keeps the rows in
    progress.  For each row n we collect n primes p with p ≡ 1 (mod n)
    (for n=1 every prime qualifies).  After a row is finished we
    advance to the next row. The sequence is memoised: later calls
    reuse the already generated terms, so any order of requests is
    supported.  This is an O(n) search, easily fast enough for n≈10000. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *seq      = NULL;   /* stored sequence values          */
static size_t seq_len  = 0;      /* number of terms computed so far */
static size_t seq_cap  = 0;      /* current capacity of seq        */

static uint64_t *prime_list = NULL;   /* primes discovered so far     */
static size_t   prime_cnt   = 0;       /* number of primes in list      */
static size_t   prime_cap   = 0;       /* capacity of prime list        */

static uint64_t   last_prime   = 1;    /* last term added to sequence   */
static uint32_t   current_row  = 1;    /* row being processed           */
static uint32_t   primes_in_row = 0;   /* primes found in current row   */

/* ------------------------------------------------------------------ */
/*  Resize helper: double capacity when needed                         */
static void ensure_seq_cap(size_t required)
{
    if (seq_cap < required) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 128;
        while (new_cap < required) new_cap <<= 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}
static void ensure_prime_cap(size_t required)
{
    if (prime_cap < required) {
        size_t new_cap = prime_cap ? prime_cap << 1 : 64;
        while (new_cap < required) new_cap <<= 1;
        prime_list = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        prime_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/*  Simple deterministic primality test using the prime list          */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2)          return false;
    if (x == 2)         return true;
    if ((x & 1ULL) == 0U) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined; return 0 for safety */

    /* Ensure we have at least n terms computed */
    while (seq_len < n) {
        /* Find the next prime that satisfies the current row rule */
        uint64_t candidate = last_prime + 1;
        if (candidate <= 2) candidate = 2;
        if (candidate > 2 && (candidate & 1ULL) == 0U) ++candidate;   /* skip evens */

        for (;;) {
            if (candidate <= 2) candidate = 2;
            if (candidate > 2 && (candidate & 1ULL) == 0U) { ++candidate; continue; }

            if (!is_prime_uint64(candidate)) { ++candidate; continue; }

            /* Add prime to our list of known primes */
            if (prime_cnt == 0 || prime_list[prime_cnt-1] != candidate) {
                ensure_prime_cap(prime_cnt + 1);
                prime_list[prime_cnt++] = candidate;
            }

            /* Does it belong to the current row? */
            if (current_row == 1 || (candidate % current_row) == 1U) {
                /* Store term */
                ensure_seq_cap(seq_len + 1);
                seq[seq_len++] = (seq_t)candidate;

                ++primes_in_row;
                if (primes_in_row == current_row) {
                    ++current_row;
                    primes_in_row = 0;
                }
                last_prime = candidate;
                ++candidate;  /* move past this prime for next search */
                break;         /* next outer iteration (maybe new row) */
            }
            /* Prime does not fit current row – skip it */
            ++candidate;    /* keep searching */
        }
    }
    return seq[n - 1];
}
