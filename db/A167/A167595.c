/*
 * A167595  The number of nonisolated primes between the n-th nonisolated
 *          nonprime and the n-th isolated nonprime.
 * https://oeis.org/A167595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of nonisolated primes between the n‑th
 * nonisolated nonprime and the n‑th isolated nonprime.
 * A nonisolated prime is a prime that has a twin (p±2 prime).
 * An isolated nonprime is a composite number N such that
 * N−1 and N+1 are both prime.
 * A nonisolated nonprime is any nonprime that is not isolated.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* ---------- static state ---------- */
static const idx_t MAX_SIEVE = 20000000;      /* sieve upper bound                 */
static bool *is_prime = NULL;                 /* primality table                    */

static size_t prev_cap = 0;                   /* capacity of prev_counts            */
static idx_t *prev_counts = NULL;             /* prefix counts of nonisolated primes
                                               at each nonisolated nonprime            */
static size_t ans_cap = 0;                    /* capacity of answers                */
static seq_t *answers = NULL;                 /* a(n) values, 1‑based               */
static idx_t cur_i = 0;                       /* current integer being examined     */
static size_t A_seen = 0;                     /* number of nonisolated nonprimes seen */
static size_t B_seen = 0;                     /* number of isolated nonprimes seen   */
static idx_t count_non_iso_primes = 0;        /* running count of nonisolated primes */

/* ---------- helpers ---------- */
static void ensure_sieve(void)
{
    if (is_prime != NULL)
        return;

    is_prime = (bool *)malloc((MAX_SIEVE + 5) * sizeof(bool));
    if (!is_prime)
        exit(1);

    for (idx_t i = 0; i <= MAX_SIEVE + 4; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (idx_t p = 2; p * p <= MAX_SIEVE; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= MAX_SIEVE; j += p)
                is_prime[j] = false;
        }
    }
}

static void ensure_prev_capacity(size_t need)
{
    if (prev_cap >= need)
        return;
    size_t new_cap = prev_cap ? prev_cap << 1 : 64;
    if (new_cap < need)
        new_cap = need;
    idx_t *tmp = (idx_t *)realloc(prev_counts, new_cap * sizeof(idx_t));
    if (!tmp)
        exit(1);
    prev_counts = tmp;
    prev_cap = new_cap;
}

static void ensure_ans_capacity(size_t need)
{
    if (ans_cap >= need)
        return;
    size_t new_cap = ans_cap ? ans_cap << 1 : 64;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)realloc(answers, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);
    answers = tmp;
    ans_cap = new_cap;
}

/* Expand pre‑computed data until we have at least "target" terms. */
static void grow_to(idx_t target)
{
    /* We already know B_seen counts how many answers are available. */
    if (B_seen >= target)
        return;

    ensure_sieve();

    while (B_seen < target) {
        bool prime = is_prime[cur_i];

        if (!prime) {
            bool isolated = (cur_i >= 2) && is_prime[cur_i - 1] && is_prime[cur_i + 1];
            if (isolated) {
                /* Isolated nonprime (B). */
                ++B_seen;
                ensure_ans_capacity(B_seen);
                /* Number of nonisolated primes between current A and this B. */
                answers[B_seen - 1] = (seq_t)(count_non_iso_primes - prev_counts[B_seen - 1]);
            } else {
                /* Nonisolated nonprime (A). */
                ++A_seen;
                ensure_prev_capacity(A_seen);
                prev_counts[A_seen - 1] = count_non_iso_primes;
            }
        } else if (cur_i > 2) {
            /* Prime: check if it is part of a twin pair. */
            bool left_twin  = (cur_i >= 2)      ? is_prime[cur_i - 2] : false;
            bool right_twin = (cur_i + 2 <= MAX_SIEVE) ? is_prime[cur_i + 2] : false;
            if (left_twin || right_twin)
                ++count_non_iso_primes;
        }

        ++cur_i;
        /* Avoid going past the sieve limits. */
        if (cur_i > MAX_SIEVE) {
            /* Enlarge the sieve if necessary (unlikely for n <= 10000). */
            const idx_t new_max = MAX_SIEVE * 2;
            bool *new_table = (bool *)malloc((new_max + 5) * sizeof(bool));
            if (!new_table)
                exit(1);
            for (idx_t i = 0; i <= new_max + 4; ++i)
                new_table[i] = true;
            new_table[0] = new_table[1] = false;
            for (idx_t p = 2; p * p <= new_max; ++p) {
                if (new_table[p]) {
                    for (idx_t j = p * p; j <= new_max; j += p)
                        new_table[j] = false;
                }
            }
            free(is_prime);
            is_prime = new_table;
        }
    }
}

/* ---------- public interface ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n=0, but return 0 safely */
    grow_to(n);
    return answers[n - 1];
}
