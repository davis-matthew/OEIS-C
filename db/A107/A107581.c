/*
 * A107581  Numbers n such that (n + prime(n)), (n+1 + prime(n+1)) and (n+2 +
 *          prime(n+2)) are divisible by 5.
 * https://oeis.org/A107581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the sequence A107581: the numbers n such that
 * (n + prime(n)), (n+1 + prime(n+1)), and (n+2 + prime(n+2)) are all
 * divisible by 5.  The implementation uses a sieve to pre‑compute
 * primes up to a fixed limit and then searches for the desired terms.
 * Results are cached so that repeated calls are fast. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>     /* for size_t */

/* ---------- prime sieve (once) ---------- */
static uint32_t *primes = NULL;          /* array of primes */
static size_t primes_cnt = 0;            /* number of primes stored */

static void init_primes(void)
{
    if (primes != NULL) return;          /* already initialized */

    /* Prime upper bound – sufficient for n up to at least 10 000 terms. */
    const uint32_t LIMIT = 30000000U;     /* 3·10⁷ */

    /* Sieve array */
    uint8_t *is_comp = (uint8_t *)calloc(LIMIT + 1, 1);   /* 0 = prime candidate */
    if (!is_comp) return;                    /* memory failure – silently abort */

    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            for (uint64_t j = (uint64_t)i * i; j <= LIMIT; j += i)
                is_comp[(size_t)j] = 1;
        }
    }

    /* Count primes */
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[(size_t)i]) ++primes_cnt;

    /* Allocate array */
    primes = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
    if (!primes) { free(is_comp); return; }

    /* Fill array */
    size_t idx = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[(size_t)i]) primes[idx++] = i;

    free(is_comp);
}

/* ---------- sequence storage ---------- */
static seq_t *seq_vals = NULL;   /* array of found terms */
static idx_t seq_used = 0;       /* number of terms stored */
static idx_t seq_cap = 0;        /* capacity of seq_vals */

/* Ensure capacity for at least min_needed elements. */
static void ensure_seq_cap(idx_t min_needed)
{
    if (seq_cap >= min_needed) return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : 32;
    while (new_cap < min_needed) new_cap *= 2;
    seq_vals = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ---------- memoized state for generation ---------- */
static idx_t last_candidate = 0;  /* last n examined */

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for offset 1, avoid UB */

    init_primes();

    /* Generate terms until we have at least n of them */
    while (seq_used < n) {
        ++last_candidate;  /* new candidate n */

        /* We need prime(n), prime(n+1), prime(n+2) */
        if ((size_t)last_candidate + 1 >= primes_cnt) {
            /* Should not happen with the chosen LIMIT */
            break;
        }

        uint32_t p0 = primes[(size_t)(last_candidate - 1)];
        uint32_t p1 = primes[(size_t)last_candidate];
        uint32_t p2 = primes[(size_t)(last_candidate + 1)];

        /* Check divisibility by 5 */
        uint64_t s0 = (uint64_t)last_candidate + p0;
        uint64_t s1 = (uint64_t)(last_candidate + 1) + p1;
        uint64_t s2 = (uint64_t)(last_candidate + 2) + p2;

        if (s0 % 5ULL != 0ULL) continue;
        if (s1 % 5ULL != 0ULL) continue;
        if (s2 % 5ULL != 0ULL) continue;

        /* Satisfies all three conditions – store it */
        ensure_seq_cap(seq_used + 1);
        seq_vals[seq_used++] = (seq_t)last_candidate;
    }

    return seq_vals[n - 1];
}
