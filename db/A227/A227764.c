/*
 * A227764  Losing positions in the misere version of the Subtract-a-Prime
 *          game.
 * https://oeis.org/A227764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the n-th losing position in the misère subtract‑a‑prime game.
    The function iterates board positions, determines win/lose using a
    pre‑computed list of primes, and stores the losing positions
    sequentially.  Prime generation is performed on the fly as needed.
    The algorithm is O(N · π(N)) for the largest N requested, which
    is comfortably below the allowed time for N ≈ 10000. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include "../utils/build_primes_upto.h"

static seq_t *losing = NULL;          /* array of losing positions (seq_t)   */
static size_t losing_cnt = 0;         /* number of stored losing positions   */
static size_t losing_cap = 0;         /* allocated capacity of losing        */

static bool *win = NULL;              /* win[0 .. processed]                 */
static size_t win_cap = 0;            /* capacity of win array               */
static size_t processed = 0;          /* largest positional index processed  */

static uint64_t *primes = NULL;       /* list of primes up to primes_limit   */
static size_t primes_cnt = 0;
static uint64_t primes_limit = 0;     /* largest prime generated            */

/* Ensure that win[] has room for indices up to up_to (inclusive).   */
static void win_ensure_cap(size_t up_to)
{
    if (up_to < win_cap) return;
    size_t new_cap = win_cap ? win_cap << 1 : 8;
    while (new_cap <= up_to) new_cap <<= 1;
    bool *tmp = (bool *)realloc(win, new_cap * sizeof(bool));
    assert(tmp);
    win = tmp;
    win_cap = new_cap;
}

/* Ensure that losing[] can store n elements.                         */
static void losing_ensure_cap(size_t n)
{
    if (n <= losing_cap) return;
    size_t new_cap = losing_cap ? losing_cap << 1 : 16;
    while (new_cap < n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(losing, new_cap * sizeof(seq_t));
    assert(tmp);
    losing = tmp;
    losing_cap = new_cap;
}

/* Ensure prime list covers all primes ≤ limit.                       */
static void primes_ensure_upto(uint64_t limit)
{
    if (limit <= primes_limit) return;
    free(primes);
    build_primes_upto(limit, &primes, &primes_cnt);
    primes_limit = limit;
}

/* Compute losing positions until we have at least n of them.        */
static void compute_up_to_nth_losing(idx_t n)
{
    if (n <= (idx_t)losing_cnt) return;          /* already have it   */

    /* Initialise base positions if this is the first call. */
    if (processed == 0) {
        win_ensure_cap(1);          /* indices 0 and 1 needed */
        win[0] = true;              /* terminal positions are winning (misère)   */
        win[1] = true;
        processed = 1;
    }

    /* Extend computation until we have n losing positions. */
    while (losing_cnt < (size_t)n) {
        size_t pos = processed + 1;   /* next position to handle           */
        win_ensure_cap(pos);
        primes_ensure_upto((uint64_t)pos);

        bool is_win = false;
        /* try every prime ≤ pos */
        for (size_t pi = 0; pi < primes_cnt && primes[pi] <= pos; ++pi) {
            size_t prev = pos - primes[pi];
            /* prev is always ≤ processed (since primes[pi] ≥ 2) */
            if (!win[prev]) {             /* move to a losing position */
                is_win = true;
                break;
            }
        }
        win[pos] = is_win;
        if (!is_win) {                 /* position pos is losing         */
            losing_ensure_cap(losing_cnt + 1);
            losing[losing_cnt] = (seq_t)pos;
            ++losing_cnt;
        }
        processed = pos;
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    assert(n >= 1);
    compute_up_to_nth_losing(n);
    return losing[(size_t)n - 1];
}
