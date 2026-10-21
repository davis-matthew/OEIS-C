/*
 * A226039  Numbers k such that there exist primes p which divide k+1 and p-1
 *          does not divide k.
 * https://oeis.org/A226039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   build_primes, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Seq A226039 – numbers k such that ∃prime p|k+1 with (p‑1)∤k.
   The code simply tests candidate k’s one by one and records the
   qualifying values.  This brute‑force procedure is fast enough for
   n ≤ 10 000 and works for any seq_t/idx_t pair supplied by the build. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes.h"
#include "../utils/seq_realloc.h"

/* ---------- static state ---------- */
static seq_t *seq          = NULL;   /* array of found terms          */
static size_t seq_len      = 0;      /* number of terms stored        */
static size_t seq_cap      = 0;      /* allocated capacity of seq[]   */

static idx_t *prime_list   = NULL;   /* primes up to LIMIT            */
static size_t prime_cnt    = 0;      /* number of primes              */

static idx_t last_checked  = 4;      /* last integer examined (k)     */
static bool   initialized  = false;  /* whether the prime list is ready */

/* ---------- helper: resize seq[] ---------- */
static void ensure_seq_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : needed;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = seq_realloc(seq, seq_cap, new_cap);  /* seq_realloc handles NULL when old==NULL   */
    seq = tmp;
    seq_cap = new_cap;
}

/* ---------- helper: build prime table ---------- */
static void init_primes(void)
{
    if (initialized) return;
    /* A limit that exceeds the 10 000‑th term – 2 000 000 is ample.  */
    const idx_t LIMIT = 2000000U;
    build_primes(LIMIT, &prime_list, &prime_cnt);
    initialized = true;
}

/* ---------- test whether k satisfies the condition ---------- */
static bool qualifies(seq_t k)
{
    idx_t kv = (idx_t)k;            /* k fits in idx_t         */
    idx_t num = kv + 1U;            /* k + 1                   */
    idx_t temp = num;               /* copy for factorisation  */

    /* check each prime divisor of k+1 */
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p * p > temp) break;
        if (temp % p == 0U) {
            /* found prime divisor p */
            if (kv % (p - 1U) != 0U) return true;
            while (temp % p == 0U) temp /= p;
        }
    }
    /* remaining factor > sqrt(num) is prime */
    if (temp > 1U) {
        idx_t p = temp;
        if (kv % (p - 1U) != 0U) return true;
    }
    return false;
}

/* ---------- grow the sequence until it contains at least n terms ---------- */
static void grow_sequence(idx_t n)
{
    init_primes();
    ensure_seq_capacity((size_t)n);

    while (seq_len < (size_t)n) {
        ++last_checked;                         /* next candidate k  */
        seq_t cand = (seq_t)last_checked;
        if (qualifies(cand)) seq[seq_len++] = cand;
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0U) return 0;          /* not defined for n < 1, harmless   */
    grow_sequence(n);               /* generate up to the requested n    */
    return seq[n - 1U];
}
