/*
 * A024531  a(n) = [ (2nd elementary symmetric function of P(n))/(first
 *          elementary symmetric function of P(n)) ], where P(n) = {1, p(1),
 *          p(2), ..., p(n-1)}, where p(0) = 1.
 * https://oeis.org/A024531
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( (Σpᵢ + Σ_{i<j} pᵢ∙pⱼ) / (1 + Σpᵢ) ) where pᵢ =
 *            i‑th prime (p(1)=2, p(2)=3, …), and Σ ranges over p₁,…,p_{n‑1}
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ---------------------------------------------------------------------- */
/* Internal state ------------------------------------------------------- */

static uint64_t *primes      = NULL;      /* prime list (zero‑based)          */
static size_t   primes_cnt  = 0;          /* number of primes stored           */
static uint64_t  next_sieve = 2;          /* next candidate for sieve          */

static seq_t    e2_total    = 0;          /* Σ_{i<j} pᵢ∙pⱼ + Σpᵢ             */
static seq_t    S_total     = 0;          /* Σpᵢ                            */
static idx_t    processed   = 0;          /* primes processed so far          */

static seq_t  *a_vals      = NULL;       /* a(n) values for n ≥ 2            */
static size_t  a_vals_cap  = 0;           /* allocated capacity of a_vals     */
static size_t  a_vals_len  = 0;           /* number of indices actually used  */

/* ---------------------------------------------------------------------- */
/* Helper: ensure at least 'need' primes are available                    */

static void ensure_primes(idx_t need)
{
    while (primes_cnt < (size_t)need)
    {
        uint64_t cand = (primes_cnt == 0) ? 2 : next_sieve;
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i)
        {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (!is_prime)
        {
            next_sieve = (cand == 2) ? 3 : cand + 2;
            continue;
        }

        uint64_t *tmp = (uint64_t *)realloc(primes, (primes_cnt + 1) * sizeof(uint64_t));
        if (!tmp) abort();
        primes = tmp;
        primes[primes_cnt++] = cand;

        next_sieve = (cand == 2) ? 3 : cand + 2;
    }
}

/* ---------------------------------------------------------------------- */
/* Helper: ensure a_vals array can contain index 'n'.                     */

static void ensure_a_array(idx_t n)
{
    if ((size_t)(n + 1) > a_vals_cap)
    {
        size_t new_cap = a_vals_cap ? a_vals_cap << 1 : 16;
        while ((size_t)(n + 1) > new_cap) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        a_vals = tmp;
        a_vals_cap = new_cap;
    }
    if ((size_t)(n + 1) > a_vals_len) a_vals_len = n + 1;
}

/* ---------------------------------------------------------------------- */
/* Main function -------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* behaviour below offset irrelevant */

    /* Ensure we have enough primes to cover the first n‑1 primes.   */
    ensure_primes(n - 1);

    /* Ensure a_vals can store a(n). */
    ensure_a_array(n);

    /* Grow the sequence up to the required n. */
    while (processed < (n - 1))
    {
        uint64_t p = primes[processed];
        seq_t   p_seq = (seq_t)p;
        seq_t   S_prev = S_total;          /* sum before adding p */

        /* Update e2_total: add new prime’s self‑product 1* p and all
           pairwise products with previous primes. */
        e2_total += p_seq + p_seq * S_prev;

        /* Update sum and e1. */
        S_total += p_seq;
        seq_t e1 = (seq_t)1 + S_total;

        /* Store a for the current n = processed + 2. */
        idx_t cur_n = processed + 2;
        a_vals[cur_n] = e2_total / e1;

        ++processed;
    }

    return a_vals[n];
}
