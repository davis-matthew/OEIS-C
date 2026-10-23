/*
 * A173701  Let S be the sequence and let D denote the sequence obtained by
 *          moving every third term three steps to the left. The D is the
 *          sequence of absolute values of the first differences of S.
 * https://oeis.org/A173701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A173701 – lexicographically earliest permutation of the
 * natural numbers such that moving every third term three positions
 * to the left yields the sequence of absolute first differences.
 * The sequence is generated block‑wise: each block consists of three
 * consecutive values (a , b , c) with b = a + c and a_next = a + 2c.
 * For a new block the smallest unused integer c_next is chosen so that
 * a_next + c_next and a_next + 2c_next are also unused.
 * This function returns the n‑th element (1‑based) of the sequence.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>       /* realloc, abort */

/* The sequence and index types are defined by the build. */
typedef uint64_t idx_t;   /* example definition – actual build may differ */

/* Block structure: a, b = a+c, c*/
typedef struct { seq_t a; seq_t b; seq_t c; } Block;

/* ========= C implementation (for non‑C++ builds) ========= */
#ifndef __cplusplus

/* --- global state ----------------------------------------------------- */
static Block *blocks          = NULL;
static size_t blocks_len      = 0;   /* number of blocks stored */
static size_t blocks_cap      = 0;   /* capacity of blocks array      */
static bool  *used_bits       = NULL;/* bitset of used numbers         */
static size_t used_bits_size  = 0;   /* bitset capacity (in entries)   */
static bool   initialized     = false;/*  state of the tables           */

/* --- bitset helpers --------------------------------------------------- */
static inline void used_init(void)
{
    used_bits      = NULL;
    used_bits_size = 0;
}

static inline void used_add(seq_t v)
{
    size_t val = (size_t)v;     /* numbers are small enough for size_t   */
    if (val >= used_bits_size) {
        size_t newsize = used_bits_size ? used_bits_size << 1 : 8UL;
        while (newsize <= val) newsize <<= 1;
        bool *tmp = (bool *)realloc(used_bits, newsize * sizeof(bool));
        if (!tmp) abort();
        for (size_t i = used_bits_size; i < newsize; ++i) tmp[i] = false;
        used_bits     = tmp;
        used_bits_size = newsize;
    }
    used_bits[(size_t)v] = true;
}

static inline bool used_contains(seq_t v)
{
    size_t val = (size_t)v;
    return val < used_bits_size && used_bits[val];
}

/* --- block array helpers --------------------------------------------- */
static inline void blocks_ensure_cap(size_t need)
{
    if (blocks_cap < need) {
        size_t newcap = blocks_cap ? blocks_cap << 1 : 8UL;
        while (newcap < need) newcap <<= 1;
        Block *tmp = (Block *)realloc(blocks, newcap * sizeof(Block));
        if (!tmp) abort();
        blocks = tmp;
        blocks_cap = newcap;
    }
}

/* --- block generation -------------------------------------------------- */
static void generate_blocks_until(size_t target)
{
    while (blocks_len < target) {
        const Block cur = blocks[blocks_len - 1];
        seq_t a_next = cur.a + (seq_t)2 * cur.c;

        seq_t cand = 1;
        for (;;) {
            if (!used_contains(cand) &&
                !used_contains(a_next + cand) &&
                !used_contains(a_next + (seq_t)2 * cand))
                break;
            ++cand;
        }

        seq_t c_next = cand;
        seq_t b_next = a_next + c_next;

        used_add(a_next);
        used_add(c_next);
        used_add(b_next);

        blocks_ensure_cap(blocks_len + 1);
        blocks[blocks_len].a = a_next;
        blocks[blocks_len].b = b_next;
        blocks[blocks_len].c = c_next;
        ++blocks_len;
    }
}

#endif /* !__cplusplus */

/* ========= C++ implementation (for arbitrary‑precision builds) ======== */
#ifdef __cplusplus
#include <vector>
#include <set>

static std::vector<Block> blocks;   /* blocks[0] = first block          */
static std::set<seq_t> used_set;    /* set of used numbers              */
static bool initialized = false;

static void generate_blocks_until(size_t target)
{
    while (blocks.size() < target) {
        const Block &cur = blocks.back();
        seq_t a_next = cur.a + (seq_t)2 * cur.c;

        seq_t cand = 1;
        for (;;) {
            if (!used_set.count(cand) &&
                !used_set.count(a_next + cand) &&
                !used_set.count(a_next + (seq_t)2 * cand))
                break;
            ++cand;
        }

        seq_t c_next = cand;
        seq_t b_next = a_next + c_next;

        used_set.insert(a_next);
        used_set.insert(c_next);
        used_set.insert(b_next);

        blocks.push_back(Block{a_next, b_next, c_next});
    }
}

#endif /* __cplusplus */

/* ========= main function a(idx_t n) ----------------------------------- */
seq_t a(idx_t n)
{
    assert(n >= 1);

    if (!initialized) {
#ifdef __cplusplus
        blocks.reserve(8);
        blocks.push_back(Block{1, 3, 2});
        used_set.insert(1);
        used_set.insert(3);
        used_set.insert(2);
#else
        blocks_ensure_cap(1);
        blocks[0].a = 1;
        blocks[0].b = 3;
        blocks[0].c = 2;
        blocks_len = 1;
        used_init();
        used_add(1);
        used_add(3);
        used_add(2);
#endif
        initialized = true;
    }

    /* number of blocks needed to reach position n (1‑based) */
    size_t needed_blocks = (n + 2) / 3;    /* ceil(n/3) */

#ifdef __cplusplus
    generate_blocks_until(needed_blocks);
#else
    generate_blocks_until(needed_blocks);
#endif

    size_t block_idx = (n - 1) / 3;    /* 0‑based block index   */
    size_t pos_mod  = (n - 1) % 3;    /* 0 → a, 1 → b, 2 → c  */

#ifdef __cplusplus
    const Block &blk = blocks[block_idx];
#else
    const Block blk = blocks[block_idx];
#endif

    if (pos_mod == 0) return blk.a;
    if (pos_mod == 1) return blk.b;
    return blk.c;
}
