/*
 * A118248  Least nonnegative integer whose binary representation does not
 *          occur in the concatenation of the binary representations of all
 *          earlier terms.
 * https://oeis.org/A118248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 9325, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A118248 – least non‑negative integer whose binary representation
    does not appear as a substring of the concatenation of the binary
    representations of all earlier terms.
    Implementation uses a binary suffix automaton (SAM) to track all
    substrings seen so far.  For each new term the automaton is
    extended with its binary bits, and the next term is the smallest
    integer whose binary string is not present in the SAM.  The code
    works for any seq_t/idx_t types provided by the build (unsigned
    integer, __int128, or an arbitrary‑precision class).  It uses only
    C11 features and is also valid C++17.  The function a(idx_t n) is
    memoised: any number of calls with arbitrary n are served in
    O(n) time overall.  */
/*  Needless to say, the algorithm is not a closed formula, but it
    implements the definition directly.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- State of the suffix automaton --------------------------------- */
#define MAX_STATES 600000      /* sufficient for n ≤ 10 000 */

typedef struct {
    int next[2];
    int link;
    int len;
} State;

static State sam[MAX_STATES];
static int   sam_sz   = 0;   /* number of used states */
static int   sam_last = 0;   /* last state used for new extension */
static bool  sam_init = false;

/* initialise the automaton once */
static void init_sam(void) {
    if (sam_init) return;
    sam[0].next[0] = sam[0].next[1] = -1;
    sam[0].link = -1;
    sam[0].len  = 0;
    sam_sz = 1;
    sam_last = 0;
    sam_init = true;
}

/* extend the automaton with one binary bit (bit = 0 or 1) */
static void sam_extend(int bit) {
    if (sam_sz >= MAX_STATES) abort();   /* safety check */

    int cur = sam_sz++;
    sam[cur].len = sam[sam_last].len + 1;
    sam[cur].next[0] = sam[cur].next[1] = -1;
    sam[cur].link = -1;

    int p = sam_last;
    while (p != -1 && sam[p].next[bit] == -1) {
        sam[p].next[bit] = cur;
        p = sam[p].link;
    }

    if (p == -1) {
        sam[cur].link = 0;
    } else {
        int q = sam[p].next[bit];
        if (sam[p].len + 1 == sam[q].len) {
            sam[cur].link = q;
        } else {
            /* clone state q */
            if (sam_sz >= MAX_STATES) abort();
            int clone = sam_sz++;
            sam[clone] = sam[q];
            sam[clone].len = sam[p].len + 1;
            while (p != -1 && sam[p].next[bit] == q) {
                sam[p].next[bit] = clone;
                p = sam[p].link;
            }
            sam[q].link = sam[cur].link = clone;
        }
    }
    sam_last = cur;
}

/* ---------- binary conversion utilities ----------------------------------- */
/* number of bits in x (at least 1) */
static int bitlen_seq_t(seq_t x) {
    int l = 1;
    seq_t t = x;
    while ((t >>= 1) != 0) ++l;
    return l;
}

/* check whether the binary representation of x occurs as a substring
   in the SAM (i.e., in the concatenation of earlier terms). */
static bool exists_in_sam(seq_t x) {
    int l = bitlen_seq_t(x);
    int state = 0;
    for (int i = l - 1; i >= 0; --i) {
        int bit = (int)((x >> i) & (seq_t)1);
        int nxt = sam[state].next[bit];
        if (nxt == -1) return false;
        state = nxt;
    }
    return true;
}

/* append the binary representation of x to the SAM (i.e., to the global
   concatenated string). */
static void extend_with(seq_t x) {
    int l = bitlen_seq_t(x);
    for (int i = l - 1; i >= 0; --i) {
        int bit = (int)((x >> i) & (seq_t)1);
        sam_extend(bit);
    }
}

/* ---------- sequence cache ------------------------------------------------- */
typedef struct {
    seq_t  *data;
    size_t size;
    size_t cap;
} Cache;

static Cache seq_cache = { NULL, 0, 0 };

/* grow cache capacity to at least 'need' elements */
static void cache_grow(Cache *c, size_t need) {
    if (need <= c->cap) return;
    size_t newcap = c->cap ? c->cap * 2 : 8;
    while (newcap < need) newcap <<= 1;
    seq_t *tmp = (seq_t*)realloc(c->data, newcap * sizeof(seq_t));
    if (!tmp) abort();
    c->data = tmp;
    c->cap = newcap;
}

/* ensures that a[0..n] are computed */
static void ensure_up_to(idx_t n) {
    if (n < seq_cache.size) return;
    init_sam();                /* initialise SAM */

    /* first term a(0) = 0 */
    if (seq_cache.size == 0) {
        cache_grow(&seq_cache, 1);
        seq_cache.data[0] = (seq_t)0;
        seq_cache.size = 1;
        extend_with((seq_t)0);
    }

    seq_t next_candidate = (seq_t)1;   /* next integer to test */
    /* remember last candidate used for next call */
    static bool has_last_candidate = false;
    static seq_t last_candidate = 0;
    if (has_last_candidate) next_candidate = last_candidate;

    while (seq_cache.size <= n) {
        /* find next missing integer */
        seq_t cand = next_candidate;
        while (exists_in_sam(cand)) {
            ++cand;
        }
        /* store new term */
        cache_grow(&seq_cache, seq_cache.size + 1);
        seq_cache.data[seq_cache.size++] = cand;
        extend_with(cand);
        /* prepare next candidate */
        next_candidate = cand + 1;
    }
    /* keep last_candidate for subsequent non‑increasing calls */
    has_last_candidate = true;
    last_candidate = next_candidate;
}

/* ---------- required public function ------------------------------------- */
seq_t a(idx_t n) {
    ensure_up_to(n);
    return seq_cache.data[(size_t)n];
}
