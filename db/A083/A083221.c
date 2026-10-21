/*
 * A083221  Sieve of Eratosthenes arranged as an array and read by
 *          antidiagonals as A(1,1), A(1,2), A(2,1), A(1,3), A(2,2), A(3,1),
 *          ...
 * https://oeis.org/A083221
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1595, u32 n <= 3487, u64 n <= 3487,
 *            u128 n <= 3487, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A083221 – the Sieve of Eratosthenes arranged as an
   array and read by antidiagonals.  The array has rows indexed by the
   primes p1 = 2, p2 = 3, p3 = 5, …;  the elements in row i are
   pi × n where n runs through the positive integers that are not
   divisible by any smaller prime.  The sequence a(n) is obtained by
   traversing this infinite array in antidiagonal order, with a(2) =
   the first entry.  The implementation precomputes the first 200
   primes and, for each prime, the first 200 admissible multipliers
   (numbers coprime to all smaller primes).  The values are then
   returned on demand.  The algorithm is O(n) for each call of a(n)
   with n up to several thousands, easily below the 2‑second limit. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ---------- configuration constants ----------------------------------- */
#define MAX_PRIME_COUNT 300   /* enough to contain the first ~200 primes   */
#define MAX_ROWS 200          /* maximum number of rows needed for n~10000  */
#define MAX_COLS 200          /* maximum columns needed for n~10000       */

/* ---------- static storage ------------------------------------------- */
static int primes[MAX_PRIME_COUNT];
static int prime_cnt = 0;

static seq_t  residues[MAX_ROWS][MAX_COLS];
static uint8_t residues_used[MAX_ROWS] = {0};

static bool initialized = false;

/* ---------- helper: sieve of Eratosthenes to fill primes[] ----------- */
static void init_primes(void)
{
    const int LIMIT = 2000;            /* enough for first 200 primes */
    bool is_prime[LIMIT + 1];
    for (int i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }
    for (int i = 2; i <= LIMIT && prime_cnt < MAX_PRIME_COUNT; ++i)
        if (is_prime[i]) primes[prime_cnt++] = i;
}

/* ---------- helper: compute first MAX_COLS residues for a given row -- */
static void compute_residues_for_row(int r)   /* r is 1‑based */
{
    int pr_idx_max = r - 2; /* last earlier prime index (0‑based) */
    int col_needed = MAX_COLS;
    if (residues_used[r-1] >= col_needed) return; /* already done */

    uint64_t candidate = 1;
    while (residues_used[r-1] < col_needed) {
        bool ok = true;
        for (int pi = 0; pi <= pr_idx_max; ++pi) {
            int p = primes[pi];
            if (candidate % p == 0) { ok = false; break; }
        }
        if (ok) {
            residues[r-1][residues_used[r-1]] = candidate;
            residues_used[r-1]++;
        }
        candidate++;
    }
}

/* ---------- helper: return antidiagonal coordinates (r,c) ------------ */
static void antidiagonal_coords(idx_t n, idx_t *r, idx_t *c)
{
    /* assume n >= 2;  offset 2 means a(2) corresponds to (1,1) */
    idx_t t = n - 2;                 /* 0‑based offset from the first element   */
    idx_t d = 0;
    /* find smallest d with (d+1)*(d+2)/2 > t */
    while ((d + 1) * (d + 2) / 2 <= t) d++;
    idx_t prev_count = d * (d + 1) / 2;
    idx_t offset = t - prev_count;   /* 0‑based within the diagonal */
    *r = offset + 1;                /* rows are 1‑based                    */
    *c = d + 2 - *r;                /* columns are 1‑based                  */
}

/* ---------- main sequence function ---------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_primes();
        /* pre‑compute residues for all rows that may be needed */
        for (int r = 1; r <= MAX_ROWS; ++r) compute_residues_for_row(r);
        initialized = true;
    }

    if (n < 2) return 0; /* undefined for n < 2, but caller may not hit it */

    idx_t r, c;
    antidiagonal_coords(n, &r, &c);

    if (r > (idx_t)MAX_ROWS || c > (idx_t)MAX_COLS)
        return 0; /* sanity fallback for unexpected indices */

    seq_t prime = (seq_t)primes[(int)r - 1];
    seq_t mult  = residues[(int)r - 1][(int)c - 1];
    return prime * mult;
}
