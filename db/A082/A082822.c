/*
 * A082822  Triangle (with distinct entries) read by rows in which row n
 *          contains n numbers whose product == 1 (mod n).
 * https://oeis.org/A082822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A082822.
   Triangle (with distinct entries) read by rows: row n contains n
   distinct integers coprime to n whose product ≡ 1 (mod n).
   Numbers are selected sequentially, ensuring no duplicates in the whole
   triangle.  The sequence a(n) is the n-th entry of that triangle. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ---------- Auxiliary utilities ---------- */

static idx_t gcd_u64(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static uint64_t modinv_uint64(uint64_t a, uint64_t mod)
/* Return modular inverse of a modulo mod, assuming gcd(a,mod)=1 */
{
    int64_t t  = 0, newt = 1;
    int64_t r  = mod, newr = a % mod;
    while (newr != 0) {
        uint64_t q = r / newr;
        int64_t tmp;
        tmp = t - (int64_t)q * newt; t = newt; newt = tmp;
        tmp = r - (int64_t)q * newr; r = newr; newr = tmp;
    }
    if (r > 1) return 0;           /* inverse does not exist */
    if (t < 0) t += mod;
    return (uint64_t)t;
}

/* ---------- Global state ---------- */

static size_t seq_cap    = 0;      /* capacity of seq_vals */
static seq_t *seq_vals   = NULL;   /* sequence values, seq_vals[0] unused */
static size_t seq_len    = 0;      /* number of terms stored (excluding dummy) */

static size_t used_cap   = 0;      /* capacity of used array */
static uint8_t *used     = NULL;   /* used[v] == 1 if v already used in the triangle */

static idx_t current_row = 1;      /* last fully generated row */

/* Ensure that the used array can hold index v. */
static void ensure_used_capacity(idx_t v)
{
    if (v < used_cap) return;
    size_t new_cap = used_cap ? used_cap << 1 : 64;
    while (new_cap <= v) new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(used, new_cap);
    if (!tmp) exit(EXIT_FAILURE);        /* out of memory */
    memset(tmp + used_cap, 0, new_cap - used_cap);
    used = tmp;
    used_cap = new_cap;
}

/* Ensure that the seq_vals array can hold at least need elements
   (including the dummy at index 0). */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 64;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);        /* out of memory */
    seq_vals = tmp;
    seq_cap = new_cap;
}

/* ---------- Row generation ---------- */

static void generate_row(idx_t r)
{
    /* r >= 2 */
    size_t i;
    size_t first_needed = r - 1;
    uint64_t prod = 1;               /* product modulo r of the first r-1 numbers */

    /* Allocate temporary storage for the r numbers of this row */
    seq_t *row_vals = (seq_t *)malloc(r * sizeof(seq_t));
    if (!row_vals) exit(EXIT_FAILURE);

    /* Scan integers starting from 2 to collect first_needed coprime, unused numbers */
    idx_t m = 2;
    size_t collected = 0;
    while (collected < first_needed) {
        if (m >= used_cap) ensure_used_capacity(m);
        if (used[m]) { ++m; continue; }
        if (gcd_u64(m, r) != 1) { ++m; continue; }
        /* Accept this candidate */
        used[m] = 1;
        row_vals[collected] = (seq_t)m;
        prod = (prod * (m % r)) % r;
        ++collected;
        ++m;
    }

    /* Compute the required last number so that total product is 1 (mod r) */
    uint64_t inv = modinv_uint64(prod, (uint64_t)r); /* 1 <= inv < r */
    idx_t candidate = inv;
    if (candidate <= 1) {
        /* Bring candidate above 1 */
        idx_t add = ((2 - candidate) + r - 1) / r;
        candidate += add * r;
    }
    while (true) {
        if (candidate >= used_cap) ensure_used_capacity(candidate);
        if (!used[candidate] && gcd_u64(candidate, r) == 1) break;
        candidate += r;
    }
    used[candidate] = 1;
    row_vals[first_needed] = (seq_t)candidate;

    /* Append row values to the global sequence cache */
    size_t new_len = seq_len + r;
    ensure_seq_capacity(new_len + 1);  /* +1 for dummy index 0 */
    for (i = 0; i < r; ++i)
        seq_vals[seq_len + 1 + i] = row_vals[i];
    seq_len = new_len;
    free(row_vals);
}

/* ---------- Public interface ---------- */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* behavior below offset is irrelevant */

    /* On first call, initialize the cache with term 1 and used[1] */
    if (seq_vals == NULL) {
        seq_vals = (seq_t *)malloc(64 * sizeof(seq_t));
        if (!seq_vals) exit(EXIT_FAILURE);
        seq_cap = 64;
        seq_vals[0] = 0;           /* dummy */
        seq_len = 1;
        seq_vals[1] = (seq_t)1;
        used = (uint8_t *)malloc(64 * sizeof(uint8_t));
        if (!used) exit(EXIT_FAILURE);
        used_cap = 64;
        memset(used, 0, 64);
        used[1] = 1;
        current_row = 1;
    }

    /* Build rows until we have at least n terms */
    while (n > seq_len) {
        generate_row(current_row + 1);
        ++current_row;
    }

    return seq_vals[n];
}
