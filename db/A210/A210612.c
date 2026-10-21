/*
 * A210612  Number of nonnegative integers smaller than the largest number of
 *          Recamán's A005132 after the n-th stage that are not yet in
 *          Recamán's sequence at that stage.
 * https://oeis.org/A210612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined recursionally by simulation of Recamán’s
 *            sequence. a(1)=0. For n≥2, compute Recamán’s sequence up to the
 *            n‑th term (indexing with a0=0, a1=1, …). Let L be the largest
 *            value seen up to that stage. Let V be the number of *distinct*
 *            values ≤ L – 1 that have appeared. Then a(n) = L –...
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- Static state ---------- */
static seq_t *recam = NULL;            /* Recamán values */
static size_t recam_cap = 0;
static size_t recam_len = 0;           /* number of Recamán terms computed */

static seq_t *answers = NULL;          /* a(n) values for n≥1, 0‑based index */
static size_t answers_cap = 0;
static size_t answers_len = 0;         /* number of a(n) computed */

static seq_t *visited_vals = NULL;     /* array of distinct visited Recamán values */
static size_t visited_cap = 0;
static size_t visited_cnt = 0;         /* number of distinct visited values */

static seq_t current_max = 0;          /* largest Recamán value seen so far */
static seq_t visited_under_max = 0;    /* count of visited values < current_max */

/* ---------- Helper functions for dynamic arrays ---------- */
static void ensure_recam_capacity(size_t needed) {
    if (recam_cap >= needed) return;
    size_t newCap = recam_cap ? recam_cap * 2 : 1;
    while (newCap < needed) newCap <<= 1;
    recam = (seq_t *)realloc(recam, newCap * sizeof(seq_t));
    if (!recam) abort();
    recam_cap = newCap;
}

static void ensure_answers_capacity(size_t needed) {
    if (answers_cap >= needed) return;
    size_t newCap = answers_cap ? answers_cap * 2 : 1;
    while (newCap < needed) newCap <<= 1;
    answers = (seq_t *)realloc(answers, newCap * sizeof(seq_t));
    if (!answers) abort();
    answers_cap = newCap;
}

static void ensure_visited_capacity(size_t needed) {
    if (visited_cap >= needed) return;
    size_t newCap = visited_cap ? visited_cap * 2 : 1;
    while (newCap < needed) newCap <<= 1;
    visited_vals = (seq_t *)realloc(visited_vals, newCap * sizeof(seq_t));
    if (!visited_vals) abort();
    visited_cap = newCap;
}

/* Linear search for distinct visited values */
static bool is_visited(seq_t x) {
    for (size_t i = 0; i < visited_cnt; ++i)
        if (visited_vals[i] == x)
            return true;
    return false;
}

/* ---------- Initialization ---------- */
static void init(void) {
    if (recam != NULL) return; /* already initialized */

    /* Recamán initial term a0 = 0 */
    ensure_recam_capacity(1);
    recam[0] = 0;
    recam_len = 1;

    /* Answers start empty */
    ensure_answers_capacity(1);
    answers_len = 0;

    /* Visited values start with 0 only */
    ensure_visited_capacity(1);
    visited_vals[0] = 0;
    visited_cnt = 1;
    visited_under_max = 0;          /* no numbers < current_max (which is 0) */
    current_max = 0;
}

/* ---------- Main iterative computation ---------- */
static void compute_to(idx_t n) {
    init();                     /* Ensure state exists */

    while (answers_len < n) {
        /* Compute next Recamán term */
        idx_t nextIdx = (idx_t)recam_len;      /* 1‑based index of new term */
        seq_t last = recam[recam_len - 1];
        seq_t new_val;

        if (last > nextIdx) {
            seq_t cand = last - nextIdx;      /* subtraction safe: last > nextIdx */
            if (cand > 0 && !is_visited(cand))
                new_val = cand;
            else
                new_val = last + nextIdx;
        } else {
            new_val = last + nextIdx;
        }

        /* Update visited set and bookkeeping only if this value is new */
        bool distinct = !is_visited(new_val);
        if (distinct) {
            ensure_visited_capacity(visited_cnt + 1);
            visited_vals[visited_cnt] = new_val;
            visited_cnt++;

            if (new_val > current_max) {
                /* All previously visited numbers are < new_val */
                visited_under_max = visited_cnt - 1;   /* exclude new_val */
                current_max = new_val;
            } else if (new_val < current_max) {
                visited_under_max++;
            }
            /* new_val == current_max cannot happen for a distinct value */
        }

        /* Store Recamán value for next iteration */
        ensure_recam_capacity(recam_len + 1);
        recam[recam_len] = new_val;
        recam_len++;

        /* Compute a(n) for this stage */
        seq_t ans = current_max - visited_under_max;
        ensure_answers_capacity(answers_len + 1);
        answers[answers_len] = ans;
        answers_len++;
    }
}

/* ---------- Public API required by the problem ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* n=0 is outside the official offset */
    compute_to(n);
    return answers[n - 1];
}
