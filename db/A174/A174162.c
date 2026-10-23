/*
 * A174162  a(1) = 2. Let k >= 1 be the minimal integer such that 2*k*a(n-1) +
 *          1 has at least one prime divisor which is not already in the
 *          sequence. Then a(n) is the smallest such divisor.
 * https://oeis.org/A174162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; for n > 1, a(n) is the smallest unused prime divisor *
 *            of 2*k*a(n-1)+1, where k ≥ 1 is the minimal integer that yields
 *            such a divisor
 * Range:     u8 n <= 12, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Parameters ------------------------------------------------------- */
/* ------------------------------------------------------------------ */
#define PRIMELIMIT 2000000   /* upper bound for primes handled by the sieve */

/* ------------------------------------------------------------------ */
/* Prime sieve ------------------------------------------------------- */
/* ------------------------------------------------------------------ */
static bool *prime_flag = NULL;         /* is prime? */
static uint32_t *prime_list = NULL;     /* list of primes up to PRIMELIMIT */
static size_t prime_count = 0;

/* initialise the sieve (once) ------------------------------------ */
static void init_primes(void)
{
    if (prime_flag != NULL) return; /* already done */

    size_t lim = PRIMELIMIT;
    prime_flag = (bool*)calloc(lim + 1, sizeof(bool));
    for (size_t i = 2; i <= lim; ++i) prime_flag[i] = true;
    for (size_t i = 2; i * i <= lim; ++i) {
        if (prime_flag[i]) {
            for (size_t j = i * i; j <= lim; j += i)
                prime_flag[j] = false;
        }
    }

    /* build prime list */
    prime_count = 0;
    for (size_t i = 2; i <= lim; ++i)
        if (prime_flag[i]) ++prime_count;
    prime_list = (uint32_t*)malloc(prime_count * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 2; i <= lim; ++i)
        if (prime_flag[i]) prime_list[idx++] = (uint32_t)i;
}

/* ------------------------------------------------------------------ */
/* Seen primes ------------------------------------------------------- */
/* ------------------------------------------------------------------ */
static bool *seen_flag = NULL;          /* seen flags for primes ≤ PRIMELIMIT */

/* dynamically grow list of large primes that are seen */
static seq_t *seen_big = NULL;
static size_t seen_big_len = 0, seen_big_cap = 0;

static bool is_seen_big(seq_t p)
{
    for (size_t i = 0; i < seen_big_len; ++i)
        if (seen_big[i] == p) return true;
    return false;
}

static void add_seen_big(seq_t p)
{
    if (seen_big_len == seen_big_cap) {
        size_t new_cap = seen_big_cap ? seen_big_cap * 2 : 16;
        seen_big = (seq_t*)realloc(seen_big, new_cap * sizeof(seq_t));
        seen_big_cap = new_cap;
    }
    seen_big[seen_big_len++] = p;
}

static void init_seen(void)
{
    if (seen_flag != NULL) return; /* already done */

    seen_flag = (bool*)calloc(PRIMELIMIT + 1, sizeof(bool));
    seen_flag[2] = true;            /* a(1) = 2 is already used */

    seen_big = NULL;
    seen_big_len = seen_big_cap = 0;
}

static void mark_used(seq_t p)
{
    if (p <= PRIMELIMIT) {
        seen_flag[(uint32_t)p] = true;
    } else {
        if (!is_seen_big(p))
            add_seen_big(p);
    }
}

/* ------------------------------------------------------------------ */
/* Sequence storage -------------------------------------------------- */
/* ------------------------------------------------------------------ */
static seq_t *seq_arr = NULL;
static size_t seq_cap = 0;  /* capacity (actual allocated size) */
static size_t seq_len = 0;  /* number of terms computed (index 1 = a(1)) */

static void init_seq(void)
{
    if (seq_arr != NULL) return; /* already done */

    seq_cap = 8;
    seq_arr = (seq_t*)calloc(seq_cap, sizeof(seq_t));
    seq_arr[1] = (seq_t)2;   /* a(1) = 2 */
    seq_len = 1;
}

/* ensure capacity for index n (1‑based) */
static void ensure_seq_capacity(size_t n)
{
    /* we need at least n+1 entries (index 0 unused) */
    if (n + 1 > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        while (new_cap < n + 1) new_cap <<= 1;
        seq_arr = (seq_t*)realloc(seq_arr, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/* Main API ---------------------------------------------------------- */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_primes();
        init_seen();
        init_seq();
        initialized = true;
    }

    if (n <= seq_len) return seq_arr[n];

    ensure_seq_capacity(n);

    /* compute missing terms up to n */
    while (seq_len < n) {
        ++seq_len; /* new index */
        seq_t a_prev = seq_arr[seq_len - 1];
        seq_t chosen = 0;

        /* search k */
        for (idx_t k = 1; ; ++k) {
            /* m = 2 * k * a_prev + 1 */
            seq_t m = (seq_t)2 * a_prev;
            m = m * (seq_t)k + 1;

            seq_t tmp = m;
            bool found = false;

            /* trial division by small primes */
            for (size_t pi = 0; pi < prime_count; ++pi) {
                uint32_t sp = prime_list[pi];
                seq_t sp_seq = (seq_t)sp;

                if (sp_seq * sp_seq > tmp) break;

                if (tmp % sp_seq == 0) {
                    /* sp divides tmp */
                    if (!seen_flag[sp] && !is_seen_big((seq_t)sp)) {
                        chosen = sp_seq;
                        found = true;
                        break;
                    }
                    while (tmp % sp_seq == 0) tmp /= sp_seq;
                }
            }

            if (found) break;

            /* after loop: tmp is either 1 or a prime > sqrt(original m) */
            if (!found && tmp > 1) {
                if (tmp <= PRIMELIMIT) {
                    if (!seen_flag[(uint32_t)tmp]) {
                        chosen = tmp;
                        break;
                    }
                } else {
                    if (!is_seen_big(tmp)) {
                        chosen = tmp;
                        break;
                    }
                }
            }
            /* otherwise continue with next k */
        }

        seq_arr[seq_len] = chosen;
        mark_used(chosen);
    }

    return seq_arr[n];
}
