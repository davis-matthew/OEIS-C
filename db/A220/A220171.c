/*
 * A220171  An ordered subset of primitive values of x^2 + x*y + y^2 where at
 *          least two ordered pairs (x1,y1) and (x2,y2) with x1 != x2, y1 !=
 *          y2 and gcd(x1,y1) = gcd(x2,y2) = 1 yield identical primitive
 *          values.
 * https://oeis.org/A220171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A220171: ordered subset of primitive values of x^2 + x*y + y^2
   where at least two distinct primitive representations exist.
   The function a(n) returns the nth such number (1‑based index).
   Implementation: a sieve‑like enumeration of primitive pairs (x,y)
   with gcd(x,y)=1, keeping a hash table of occurrence counts.
   Whenever a value appears for the second time it is inserted
   into a sorted list of incident values.  The list is expanded as needed.
   Complexity is roughly O(X^2) for X the largest |x| processed,
   which is acceptable for n up to ~10⁴. 
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* --- Hash table for counting occurrences --------------------------------------------------- */
#define HASH_TABLE_SIZE (1U << 23)          /* 8,388,608 entries, power of two */

static seq_t *hash_key   = NULL;           /* key array (seq_t values)   */
static uint8_t *hash_cnt = NULL;           /* count array                */
static size_t hash_mask = 0;               /* mask = size - 1            */

static void init_hash(void)
{
    if (hash_key) return;                  /* already initialised */
    hash_mask  = HASH_TABLE_SIZE - 1;
    hash_key   = (seq_t *)        malloc(HASH_TABLE_SIZE * sizeof(seq_t));
    hash_cnt   = (uint8_t *)     malloc(HASH_TABLE_SIZE * sizeof(uint8_t));
    if (!hash_key || !hash_cnt) exit(1);
    /* set counts to 0; key values are irrelevant until used */
    memset(hash_cnt, 0, HASH_TABLE_SIZE * sizeof(uint8_t));
}

/* simple hash: lower 64 bits multiplied by a 64‑bit constant */
static size_t hash_value(seq_t v)
{
    uint64_t low = (uint64_t)v;             /* truncate if v > 64 bits, ok for hashing */
    return (size_t)(low * 11400714819323198485ULL);
}

/* --- List of incident values, kept sorted ----------------------------------------------- */
static seq_t *incidents = NULL;            /* array of values  */
static size_t incidents_size = 0;          /* number of elements   */
static size_t incidents_cap  = 0;          /* allocated capacity   */

static void ensure_incidents_cap(void)
{
    if (incidents_size >= incidents_cap) {
        incidents_cap = incidents_cap == 0 ? 128 : incidents_cap * 2;
        seq_t *newp = (seq_t *)realloc(incidents, incidents_cap * sizeof(seq_t));
        if (!newp) exit(1);
        incidents = newp;
    }
}

/* insert value into sorted array (no duplicates) */
static void add_incident(seq_t val)
{
    ensure_incidents_cap();
    /* binary search for insertion point */
    size_t lo = 0, hi = incidents_size;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (incidents[mid] < val) lo = mid + 1;
        else hi = mid;
    }
    /* shift elements to make room (assignment; preserves object invariants) */
    for (size_t i = incidents_size; i > lo; --i) {
        incidents[i] = incidents[i-1];
    }
    incidents[lo] = val;
    incidents_size++;
}

/* --- Incremental generation of primitive pairs ----------------------------------------- */
static idx_t current_max_x = 0;   /* largest x processed so far */

/* insert a new value into hash table; add to incidents if it becomes 2 */
static void insert_value(seq_t val)
{
    size_t idx = hash_value(val) & hash_mask;
    while (hash_cnt[idx]) {
        if (hash_key[idx] == val) {
            if (hash_cnt[idx] == 1) {        /* second occurrence */
                hash_cnt[idx] = 2;
                add_incident(val);
            }
            return;
        }
        idx = (idx + 1) & hash_mask;
    }
    /* empty slot found */
    hash_key[idx] = val;
    hash_cnt[idx] = 1;
}

/* generate all primitive pairs with x in (current_max_x, max_x]  */
static void generate_upto(idx_t max_x)
{
    for (idx_t x = current_max_x + 1; x <= max_x; ++x) {
        seq_t xx = (seq_t)x;
        for (idx_t y = 0; y <= x; ++y) {
            if (gcd(xx, (seq_t)y) != 1) continue;
            seq_t yy = (seq_t)y;
            seq_t n = xx * xx + xx * yy + yy * yy;
            insert_value(n);
        }
    }
    current_max_x = max_x;
}

/* ensure we have at least n incidents computed */
static void ensure_nth(idx_t n)
{
    while (incidents_size < n) {
        idx_t next_max = current_max_x + 1000;   /* step size can be tuned */
        if (next_max <= current_max_x) next_max = current_max_x + 1;
        generate_upto(next_max);
    }
}

/* --- public API ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* undefined, but guard */
    init_hash();                               /* initialise tables on first call */
    ensure_nth(n);                              /* generate until we have n incidents */
    return incidents[n - 1];                   /* index is 0‑based in array */
}
