/*
 * A100707  a(1) = 1; for n > 1, a(n+1)=a(n)-k if there exists a positive
 *          number k (take the smallest) that has not yet been used and is
 *          such that a(n+1) is new and >0, otherwise a(n+1) = a(n)+k if the
 *          same conditions are satisfied.
 * https://oeis.org/A100707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A100707 */
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* OEIS definition:
   a(1) = 1; for n > 1 let M = a(n).  Try all unused k < M in increasing
   order; if M–k is not yet in the sequence and k has not been used before,
   then a(n+1) = M–k and k is marked used.  If no such k exists,
   try all unused k in increasing order; when M+k is new, set
   a(n+1) = M+k and mark k used.  All differences k are used at most once. */

#define MAX_N 20000          /* maximal number of terms we can generate    */
#define MAX_DIFF 200000      /* maximal difference value we will need      */

static seq_t seq[MAX_N];            /* sequence values, 1‑based in terms of seq[0]*/
static size_t seq_len = 0;          /* current number of stored terms              */
static bool used_diff[MAX_DIFF+1];  /* used_diff[k] == true  ⇔  difference k was used */

static bool is_initialized = false;

/* check whether x is already present in the sequence */
static bool in_seq(seq_t x)
{
    for (size_t i = 0; i < seq_len; ++i)
        if (seq[i] == x)
            return true;
    return false;
}

/* initialise static storage on first call */
static void init_storage(void)
{
    if (is_initialized)
        return;
    seq_len = 1;
    seq[0] = (seq_t)1;
    memset(used_diff, 0, sizeof used_diff);
    is_initialized = true;
}

/* extend the sequence up to index n (1‑based) */
static void ensure_up_to(idx_t n)
{
    init_storage();
    while (seq_len < (size_t)n) {
        seq_t M = seq[seq_len - 1];
        bool found = false;
        size_t k;

        /* 1. try subtracting unused k < M */
        for (k = 1; k < (size_t)M; ++k) {
            if (k > MAX_DIFF)
                break; /* safety guard – sequence never needs such large k   */
            if (!used_diff[k]) {
                seq_t cand = M - (seq_t)k;
                if (!in_seq(cand)) {
                    seq[seq_len++] = cand;
                    used_diff[k] = true;
                    found = true;
                    break;
                }
            }
        }

        /* 2. if none found, try adding unused k */
        if (!found) {
            for (k = 1; ; ++k) {
                if (k > MAX_DIFF)
                    break; /* safety guard */
                if (!used_diff[k]) {
                    seq_t cand = M + (seq_t)k;
                    if (!in_seq(cand)) {
                        seq[seq_len++] = cand;
                        used_diff[k] = true;
                        found = true;
                        break;
                    }
                }
            }
        }
    }
}

/* public function required by the judge */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for n < 1, but safe guard */
    ensure_up_to(n);
    return seq[n - 1];
}
